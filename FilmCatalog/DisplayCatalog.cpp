#include "include/DisplayCatalog.h"

DisplayCatalog::DisplayCatalog(int id, bool edit, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::DisplayCatalogClass()), catalogID(id)
{
	ui->setupUi(this);
	QSqlQuery query(DBProvider::getInstance()->getDB());
	query.prepare("SELECT * FROM Films WHERE id = :id");
	query.bindValue(":id", catalogID);
	if (!query.exec())
	{
		qWarning() << query.lastError().text();
		return;
	}
	if (query.first())
	{
		QPixmap pix = DynamicResourceCreator::getInstance()->findFile(query.value(1).toString(), query.value(2).toString());
		file = query.value(2).toString();
		pix = pix.scaled(150, 250, Qt::KeepAspectRatio);
		ui->poster->setPixmap(pix);
		ui->release->setValue(query.value(4).toInt());
		ui->title->setText(query.value(3).toString());
		ui->duration->setText(GetTime(query.value(5).toInt()));
		ui->description->setPlainText(query.value(6).toString());
		ui->director->setText(query.value(7).toString());
		ui->actors->setText(query.value(8).toString());
		ui->studio->setText(query.value(9).toString());
		ui->genres->setText(query.value(10).toString());
	}
	if (edit)
	{
		ui->release->setReadOnly(false);
		ui->title->setReadOnly(false);
		ui->duration->setReadOnly(false);
		ui->description->setReadOnly(false);
		ui->director->setReadOnly(false);
		ui->actors->setReadOnly(false);
		ui->studio->setReadOnly(false);
		ui->genres->setReadOnly(false);
		ui->duration->setInputMask("00:00:00");
		QObject::connect(ui->accept, &QAbstractButton::clicked, [&]() {
			QFileInfo info(file);
			DynamicResourceCreator::getInstance()->addFileToPath("posters", file);
			QSqlQuery update(DBProvider::getInstance()->getDB());
			update.prepare("UPDATE Films SET PosterFile=:poster, Title=:title, Release=:release, Duration=:duration, Description=:description, Director=:director, Actors=:actors, Studio=:studio, Genres=:genres WHERE id=:id;");
			update.bindValue(":poster", info.fileName());
			update.bindValue(":title", ui->title->text());
			update.bindValue(":release", QString::number(ui->release->value()));
			QTime duration = QTime::fromString(ui->duration->text());
			update.bindValue(":duration", duration.hour() * 3600 + duration.minute() * 60 + duration.second());
			update.bindValue(":description", ui->description->toPlainText());
			update.bindValue(":director", ui->director->text());
			update.bindValue(":actors", ui->actors->text());
			update.bindValue(":studio", ui->studio->text());
			update.bindValue(":genres", ui->genres->text());
			update.bindValue(":id", catalogID);
			if (!update.exec())
			{
				qWarning() << update.lastError().text();
				return;
			}
			emit requestUpdateDisplay();
			close();
		});
		QObject::connect(ui->posterButton, &QAbstractButton::clicked, [&]() {
			QString path;
			path = QFileDialog::getOpenFileName(this, tr("Select film poster"), QString(), "Image Files (*.png *.jpg *.bmp)");
			if (path.isEmpty()) return;
			file = path;
			QPixmap pix(path);
			pix = pix.scaled(150, 250, Qt::KeepAspectRatio);
			ui->poster->setPixmap(pix);
		});
	}
	else
	{
		ui->accept->hide();
		ui->posterButton->hide();
	}
}

DisplayCatalog::~DisplayCatalog()
{
	delete ui;
}

QString DisplayCatalog::GetTime(int seconds)
{
	int minutes = seconds / 60;
	int hours = minutes / 60;
	minutes -= hours * 60;
	seconds -= hours * 3600 + minutes * 60;
	return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
