#pragma once

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "DynamicResourceCreator.h"
#include "dbprovider.h"
#include "../ui/ui_CatalogDisplayItem.h"


QT_BEGIN_NAMESPACE
namespace Ui { class CatalogDisplayItem; };
QT_END_NAMESPACE

class CatalogDisplayItem : public QWidget
{
	Q_OBJECT

public:
	CatalogDisplayItem(int id, QWidget *parent = nullptr);
	~CatalogDisplayItem();

public slots:
	void UpdateDisplay();
	void RequestDelete(int id);

private:
	Ui::CatalogDisplayItem *ui;
	QString GetTime(int seconds);
	int catalogID;
};
