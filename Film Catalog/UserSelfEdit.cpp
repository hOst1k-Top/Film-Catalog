#include "include/UserSelfEdit.h"

UserSelfEdit::UserSelfEdit(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::UserSelfEditClass())
{
	ui->setupUi(this);
	setFixedSize(size());
	login = UserProvider::getInstance()->getLogin();
	ui->nickname->setText(login);
	QObject::connect(ui->changePassword, &QAbstractButton::clicked, this, &UserSelfEdit::changePassword);
	QObject::connect(ui->exit, &QAbstractButton::clicked, [&]() {
		emit showAuth();
		close();
	});
}

UserSelfEdit::~UserSelfEdit()
{
	delete ui;
}

void UserSelfEdit::changePassword()
{
	QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
	query.prepare("UPDATE users SET Password = :pass WHERE Login = :login");
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
