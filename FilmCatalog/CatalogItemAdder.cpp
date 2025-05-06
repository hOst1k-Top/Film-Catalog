#include "include/CatalogItemAdder.h"

CatalogItemAdder::CatalogItemAdder(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CatalogItemAdderClass())
{
	ui->setupUi(this);
	QObject::connect(ui->importPoster, &QAbstractButton::clicked, [&]() {
		QString path;
		path = QFileDialog::getOpenFileName(this, tr("Select film poster"), QString(), "Image Files (*.png *.jpg *.bmp)");
		if (path.isEmpty()) return;
		filePath = path;
		QPixmap pix(path);
		pix = pix.scaled(150, 250, Qt::KeepAspectRatio);
		ui->poster->setPixmap(pix);
	});

	QObject::connect(ui->add, &QAbstractButton::clicked, [&]() {
		bool condition = ui->duration->text().isEmpty();
		condition = condition || ui->director->text().isEmpty();
		condition = condition || ui->duration->text().isEmpty();
		condition = condition || ui->actors->text().isEmpty();
		condition = condition || ui->studio->text().isEmpty();
		condition = condition || ui->genres->text().isEmpty();
		condition = condition || ui->title->text().isEmpty();
		if (condition)
		{
			QMessageBox::warning(this, tr("Error"), tr("Empty field detected"));
			return;
		}
		else
		{
			QSqlQuery insertQuery(DBProvider::getInstance()->getDB());
			insertQuery.prepare("INSERT INTO Films(PosterFile,Title,Release,Duration,Description,Director,Actors,Studio,Genres) VALUES (:poster,:title,:release,:duration,:description,:director,:actors,:studio,:genres);");
			QFileInfo info(filePath);
			DynamicResourceCreator::getInstance()->addFileToPath("posters", filePath);
			insertQuery.bindValue(":poster", info.fileName());
			insertQuery.bindValue(":title", ui->title->text());
			insertQuery.bindValue(":release", QString::number(ui->release->value()));
			QTime duration = ui->duration->time();
			insertQuery.bindValue(":duration", duration.hour() * 3600 + duration.minute() * 60 + duration.second());
			insertQuery.bindValue(":description", ui->description->toPlainText());
			insertQuery.bindValue(":director", ui->director->text());
			insertQuery.bindValue(":actors", ui->actors->text());
			insertQuery.bindValue(":studio", ui->studio->text());
			insertQuery.bindValue(":genres", ui->genres->text());
			if (!insertQuery.exec())
			{
				qWarning() << insertQuery.lastError().text();
				return;
			}
			emit requestAdding(insertQuery.lastInsertId().toInt(), ui->title->text(), QString::number(ui->release->value()), ui->genres->text().split(',', Qt::SkipEmptyParts));
			accept();
		}
	});
}

CatalogItemAdder::~CatalogItemAdder()
{
	delete ui;
}
