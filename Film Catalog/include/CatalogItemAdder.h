#pragma once

#include <QDialog>
#include <QFileDialog>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include "DynamicResourceCreator.h"
#include "dbprovider.h"
#include "../ui/ui_CatalogItemAdder.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CatalogItemAdderClass; };
QT_END_NAMESPACE

class CatalogItemAdder : public QDialog
{
	Q_OBJECT

public:
	CatalogItemAdder(QWidget *parent = nullptr);
	~CatalogItemAdder();

signals:
	void requestAdding(int id, QString title);

private:
	QString filePath;
	Ui::CatalogItemAdderClass *ui;
};
