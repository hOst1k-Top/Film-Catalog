#pragma once

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QMessageBox>
#include <QInputDialog>
#include <qcryptographichash.h>
#include "UserCard.h"
#include "dbprovider.h"
#include "UserProvider.h"
#include "../ui/ui_AdminMenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AdminMenuClass; };
QT_END_NAMESPACE

class AdminMenu : public QWidget
{
	Q_OBJECT

public:
	AdminMenu(QWidget *parent = nullptr);
	~AdminMenu();
	void SelectAll();

public slots:
	void onDeleteRequest(QString login);
	void changeAdmin(QString login, bool rights);
	void changePassword(QString login);
private:
	Ui::AdminMenuClass *ui;
};
