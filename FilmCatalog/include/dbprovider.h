#pragma once

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
#include <QFile>
#include <QString>
#include <QMap>

class DBProvider
{
public:
    DBProvider(DBProvider& other) = delete;
    void operator=(DBProvider other) = delete;
    static DBProvider* getInstance(QString dbName = "catalog.db");
    QSqlDatabase& getDB() { return db; }
    ~DBProvider() { db.close(); }

private:
    DBProvider(QString dbName);
    QSqlDatabase db;
};