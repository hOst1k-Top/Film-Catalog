#pragma once

#include <QtWidgets/QWidget>
#include "../ui/ui_Authorization.h"
#include <qsqlquery.h>
#include <qsqldatabase.h>
#include <qsqlerror>
#include <qcryptographichash.h>
#include <qmessagebox.h>
#include "dbprovider.h"
#include "UserProvider.h"
#include "FilmCatalog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class AuthorizationClass; };
QT_END_NAMESPACE

class Authorization : public QWidget
{
    Q_OBJECT

public:
    Authorization(QWidget *parent = nullptr);
    ~Authorization();

public slots:
    void RegisterUser();
    void LoginUser();
private:
    Ui::AuthorizationClass *ui;
};
