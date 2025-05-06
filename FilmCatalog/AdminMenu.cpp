#include "include/AdminMenu.h"

AdminMenu::AdminMenu(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::AdminMenuClass())
{
	ui->setupUi(this);
	SelectAll();
}

AdminMenu::~AdminMenu()
{
	delete ui;
}

void AdminMenu::SelectAll()
{
	QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
	query.exec("SELECT * FROM users");
	while (query.next())
	{
		QListWidgetItem* item = new QListWidgetItem(ui->users);
		UserCard* card = new UserCard(query.value(0).toString(), query.value(2).toBool());
		QObject::connect(card, &UserCard::deleteUser, this, &AdminMenu::onDeleteRequest);
		QObject::connect(card, &UserCard::updateAdmin, this, &AdminMenu::changeAdmin);
		QObject::connect(card, &UserCard::changePassword, this, &AdminMenu::changePassword);
		item->setData(Qt::UserRole + 1, query.value(0).toString());
		item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
		item->setSizeHint(card->minimumSizeHint());
		ui->users->addItem(item);
		ui->users->setItemWidget(item, card);
	}
}

void AdminMenu::onDeleteRequest(QString login)
{
	if (UserProvider::getInstance()->getLogin() == login)
	{
		QMessageBox::warning(this, tr("Not allowed!"), tr("You can not edit your self!"));
		return;
	}
	QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
	query.prepare("DELETE FROM users WHERE Login = :login");
	query.bindValue(":login", login);
	if (!query.exec()) QMessageBox::warning(this, tr("Failed to delete a user!"), query.lastError().text());
	for (int row = 0; row < ui->users->count(); ++row)
	{
		if (ui->users->item(row)->data(Qt::UserRole + 1).toString().contains(login))
		{
			delete ui->users->takeItem(row);
			break;
		}
	}
}

void AdminMenu::changeAdmin(QString login, bool rights)
{
	if (UserProvider::getInstance()->getLogin() == login)
	{
		QMessageBox::warning(this, tr("Not allowed!"), tr("You can not edit your self!"));
		return;
	}
	QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
	query.prepare("UPDATE users SET Admin = :isadmin WHERE Login = :login");
	query.bindValue(":login", login);
	query.bindValue(":isadmin", rights);
	query.exec();
}

void AdminMenu::changePassword(QString login)
{
	QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
	query.prepare("UPDATE users SET Hash = :pass WHERE Login = :login");
	query.bindValue(":login", login);
	QByteArray newpass = QInputDialog::getText(this, "Change password", "New password:").toUtf8();
	if (newpass.isEmpty())
	{
		QMessageBox::warning(this, tr("Wrong password format!"), tr("You enter a empty password!"));
		return;
	}
	query.bindValue(":pass", QString(QCryptographicHash::hash(newpass, QCryptographicHash::RealSha3_256).toHex()));
	if (!query.exec()) QMessageBox::warning(this, tr("Failed to change password!"), tr("Contact developer!\n") + query.lastError().text());
}
