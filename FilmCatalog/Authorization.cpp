#include "include/Authorization.h"

Authorization::Authorization(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AuthorizationClass())
{
    ui->setupUi(this);
    QObject::connect(ui->registrerButton, &QAbstractButton::clicked, this, &Authorization::RegisterUser);
    QObject::connect(ui->loginButton, &QAbstractButton::clicked, this, &Authorization::LoginUser);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::RegisterUser()
{
    QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
    query.prepare("SELECT * FROM users WHERE Login = :login");
    query.bindValue(":login", ui->login->text());
    query.exec();
    if (!query.isValid())
    {
        query.prepare("INSERT INTO users(Login, Hash) VALUES (:login, :hash)");
        query.bindValue(":login", ui->login->text());
        query.bindValue(":hash", QString(QCryptographicHash::hash(ui->password->text().toUtf8(), QCryptographicHash::RealSha3_256).toHex()));
        if (!query.exec()) QMessageBox::information(this, tr("Failed to login!"), query.lastError().text());
        else QMessageBox::information(this, tr("Account created"), tr("Congratulations! The account was successfully created."));
    }
    else QMessageBox::warning(this, tr("Failed to register a user"), tr("A user with this login already exists."));
}

void Authorization::LoginUser()
{
    QSqlQuery query = QSqlQuery(DBProvider::getInstance()->getDB());
    query.prepare("SELECT * FROM users WHERE Login = :login");
    query.bindValue(":login", ui->login->text());
    if (!query.exec()) QMessageBox::warning(this, tr("Login Error"), tr("It appears that no such user exists."));
    QCryptographicHash hashf(QCryptographicHash::Algorithm::RealSha3_256);
    while (query.next())
    {
        hashf.addData(ui->password->text().toUtf8());
        QString str1 = query.value(1).toString();
        QString str = QString(hashf.result().toHex());
        if (query.value(1).toString() == QString(hashf.result().toHex()))
        {
            UserProvider::getInstance()->setLogin(query.value(0).toString());
            UserProvider::getInstance()->setAdmin(query.value(2).toBool());
            FilmCatalog* film = new FilmCatalog();
            close();
            film->show();
            break;
        }
        else QMessageBox::warning(this, tr("Error!"), tr("Wrong Password!"));
    }
}
