#pragma once

#include <QWidget>
#include <QSqlDatabase.h>
#include <QSqlQuery>
#include <QSqlError>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QInputDialog>
#include "dbprovider.h"
#include "UserProvider.h"
#include "../ui/ui_UserSelfEdit.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserSelfEditClass; };
QT_END_NAMESPACE

class UserSelfEdit : public QWidget
{
	Q_OBJECT

public:
	UserSelfEdit(QWidget *parent = nullptr);
	~UserSelfEdit();

signals:
	void showAuth();

public slots:
	void changePassword();
private:
	Ui::UserSelfEditClass *ui;
	QString login;
};
