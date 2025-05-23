#include "include/CatalogDisplayItem.h"
#include "include/UserProvider.h"

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
	query.prepare(QString("SELECT * FROM Films WHERE id = :id"));
	query.bindValue(":id", catalogID);
	if (!query.exec()) 
	{
		qWarning() << query.lastError().text();
		return;
	}
	if (query.first())
	{
		QPixmap pix = DynamicResourceCreator::getInstance()->findFile(query.value(1).toString(), query.value(2).toString());
		pix = pix.scaled(90, 150, Qt::KeepAspectRatio);
		ui->poster->setPixmap(pix);
		ui->title->setText(query.value(3).toString());
		ui->time->setText(GetTime(query.value(5).toInt()));
		ui->genre->setText(query.value(10).toString());
	}
#ifdef FAVORITE
	query.prepare("SELECT favid FROM Favorite WHERE user = :usr AND favfilm = :id");
	query.bindValue(":usr", UserProvider::getInstance()->getLogin());
	query.bindValue(":id", catalogID);
	if (!query.exec()) qWarning() << "Query Execute Error:" << query.lastError().text();
	else if (!query.next()) ui->favorite->hide();
	else ui->favorite->show();
#endif // FAVORITE
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
