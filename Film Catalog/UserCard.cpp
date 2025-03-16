#include "include/UserCard.h"

UserCard::UserCard(QString username, bool isAdmin, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::UserCardClass())
{
	nickname = UserProvider::getInstance()->getLogin();
	admin = UserProvider::getInstance()->getAdmin();
	ui->setupUi(this);
	if (!admin) ui->giveAdmin->setIcon(this->style()->standardIcon(QStyle::SP_DialogYesButton));
	else ui->giveAdmin->setIcon(this->style()->standardIcon(QStyle::SP_DialogNoButton));
	ui->nickname->setText(nickname);
	QObject::connect(ui->removeUser, &QAbstractButton::clicked, this, [&]() {
		emit deleteUser(nickname);
	});
	QObject::connect(ui->giveAdmin, &QAbstractButton::clicked, this, [&]() {
		emit updateAdmin(nickname, !admin);
		admin = !admin;
		if (!admin) ui->giveAdmin->setIcon(this->style()->standardIcon(QStyle::SP_DialogYesButton));
		else ui->giveAdmin->setIcon(this->style()->standardIcon(QStyle::SP_DialogNoButton));
	});
	QObject::connect(ui->password, &QAbstractButton::clicked, this, [&]() {
		emit changePassword(nickname);
		});
	ui->removeUser->setIcon(this->style()->standardIcon(QStyle::SP_BrowserStop));
	ui->password->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogContentsView));
}

UserCard::~UserCard()
{
	delete ui;
}
