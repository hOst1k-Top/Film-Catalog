#include "include/CatalogDisplayItem.h"

CatalogDisplayItem::CatalogDisplayItem(int id, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::CatalogDisplayItem()), catalogID(id)
{
	ui->setupUi(this);
	UpdateDisplay();
}

CatalogDisplayItem::~CatalogDisplayItem()
{
	delete ui;
}

void CatalogDisplayItem::UpdateDisplay()
{
	QSqlQuery query(DBProvider::getInstance()->getDB());
	QMap<QString, int> columns = DBProvider::getInstance()->getColumns("Films");
	query.prepare(QString("SELECT * FROM Films WHERE id = :id").arg(columns.keys().join(",")));
	query.bindValue(":id", catalogID);
	if (!query.exec()) 
	{
		qWarning() << query.lastError().text();
		return;
	}
	if (query.first())
	{
		QPixmap pix = DynamicResourceCreator::getInstance()->findFile(query.value(columns["Path"]).toString(), query.value(columns["PosterFile"]).toString());
		pix = pix.scaled(90, 150, Qt::KeepAspectRatio);
		ui->poster->setPixmap(pix);
		ui->title->setText(query.value(columns["Title"]).toString());
		ui->time->setText(GetTime(query.value(columns["Duration"]).toInt()));
		ui->genre->setText(query.value(columns["Genres"]).toString());
	}
}

void CatalogDisplayItem::RequestDelete(int id)
{
	if (catalogID == id)
	{
		QSqlQuery query(DBProvider::getInstance()->getDB());
		query.prepare("DELETE FROM Films WHERE id = :id");
		query.bindValue(":id", catalogID);
		if (!query.exec()) qWarning() << query.lastError().text();
	}
}

QString CatalogDisplayItem::GetTime(int seconds)
{
	int minutes = seconds / 60;
	int hours = minutes / 60;
	minutes -= hours * 60;
	seconds -= hours * 3600 + minutes * 60;
	return QString("%1:%2:%3").arg(hours, 2, 10, QChar('0')).arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}
