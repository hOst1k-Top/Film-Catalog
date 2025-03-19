#include "include/DisplayCatalog.h"

DisplayCatalog::DisplayCatalog(int id, QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::DisplayCatalogClass())
{
	ui->setupUi(this);
	QSqlQuery query(DBProvider::getInstance()->getDB());
	query.prepare("SELECT * FROM Films WHERE id = :id");
	query.bindValue(":id", id);
	if (!query.exec())
	{
		qWarning() << query.lastError().text();
		return;
	}
	if (query.first())
	{
		QPixmap pix = DynamicResourceCreator::getInstance()->findFile(query.value(1).toString(), query.value(2).toString());
		pix = pix.scaled(150, 250, Qt::KeepAspectRatio);
		ui->poster->setPixmap(pix);
		ui->release->setValue(query.value(4).toInt());
		ui->titleLabel->setText(query.value(3).toString());
		ui->duration->setText(GetTime(query.value(5).toInt()));
		ui->description->setPlainText(query.value(6).toString());
		ui->director->setText(query.value(7).toString());
		ui->actors->setText(query.value(8).toString());
		ui->studio->setText(query.value(9).toString());
		ui->genres->setText(query.value(10).toString());
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
