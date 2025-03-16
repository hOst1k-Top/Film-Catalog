#pragma once

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QObject>
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
    QStringList getTables() { return header.keys(); };
    QMap<QString, int> getColumns(QString tablename);

private:
    DBProvider(QString dbName);
    QMap<QString, QMap<QString, int>> header;
    static DBProvider* instance;
    QSqlDatabase db;
};