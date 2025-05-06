#pragma once

#include <QWidget>
#include <QStyle>
#include "dbprovider.h"
#include "UserProvider.h"
#include "../ui/ui_UserCard.h"

QT_BEGIN_NAMESPACE
namespace Ui { class UserCardClass; };
QT_END_NAMESPACE

class UserCard : public QWidget
{
	Q_OBJECT

public:
	UserCard(QString username, bool isAdmin, QWidget* parent = nullptr);
	~UserCard();

signals:
	void deleteUser(QString login);
	void updateAdmin(QString login, bool admin);
	void changePassword(QString login);

private:
	Ui::UserCardClass *ui;
	QString nickname;
	bool admin;
};
