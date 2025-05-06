#pragma once

#include <QObject>
#include <QString>

class UserProvider
{
public:
	UserProvider(UserProvider& other) = delete;
	void operator=(UserProvider other) = delete;
	static UserProvider* getInstance();
	UserProvider() {};
	~UserProvider() {};
	QString getLogin() { return login; };
	void setLogin(QString login) { this->login = login; };
	bool getAdmin() { return isAdmin; };
	void setAdmin(bool AdminRights) { isAdmin = AdminRights; };

private:
	QString login;
	bool isAdmin = 0;
};