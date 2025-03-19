#pragma once

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "dbprovider.h"
#include "DynamicResourceCreator.h"
#include "../ui/ui_DisplayCatalog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DisplayCatalogClass; };
QT_END_NAMESPACE

class DisplayCatalog : public QMainWindow
{
	Q_OBJECT

public:
	DisplayCatalog(int id, QWidget *parent = nullptr);
	~DisplayCatalog();

private:
	QString GetTime(int seconds);
	Ui::DisplayCatalogClass *ui;
};
