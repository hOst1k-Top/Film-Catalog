#include "include/dbprovider.h"
DBProvider* DBProvider::instance = nullptr;

DBProvider *DBProvider::getInstance(QString dbName)
{
    if (instance == nullptr) instance = new DBProvider(dbName);
    return instance;
}

QMap<QString, int> DBProvider::getColumns(QString tablename)
{
    if (getTables().contains(tablename))
    {
        return header[tablename];
    }
    return QMap<QString, int>();
}

DBProvider::DBProvider(QString dbName)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);
    if(!db.open()) if (!db.open()) qWarning() << db.lastError().text();
    QSqlQuery create(db);
    if (!create.exec("CREATE TABLE IF NOT EXISTS \"users\" ( \"Login\" TEXT NOT NULL UNIQUE, \"Hash\" TEXT NOT NULL, \"Admin\" INTEGER NOT NULL DEFAULT 0, PRIMARY KEY(\"Login\"));"))
    {
        qWarning() << create.lastError().text();
    }
    if (!create.exec("CREATE TABLE IF NOT EXISTS \"Films\" (    \"id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"Path\" TEXT NOT NULL DEFAULT 'posters', \"PosterFile\" TEXT NOT NULL,    \"Title\" TEXT NOT NULL,    \"Release\" TEXT NOT NULL,    \"Duration\" INTEGER NOT NULL,    \"Description\" TEXT NOT NULL,    \"Director\" TEXT NOT NULL,    \"Actors\" TEXT NOT NULL,    \"Studio\" TEXT NOT NULL, \"Genres\"	TEXT NOT NULL);CREATE INDEX \"films_title_index\" ON \"Films\"(\"Title\");"))
    {
        qWarning() << create.lastError().text();
    }
#ifdef FAVORITE
    if (!create.exec("CREATE TABLE \"Favorite\" ( \"favid\" INTEGER NOT NULL, \"favfilm\" INTEGER NOT NULL, \"user\" TEXT NOT NULL, PRIMARY KEY(\"favid\" AUTOINCREMENT), FOREIGN KEY(\"favfilm\") REFERENCES \"Films\"(\"id\") ON UPDATE CASCADE ON DELETE CASCADE, FOREIGN KEY(\"user\") REFERENCES \"users\"(\"Login\") ON UPDATE CASCADE ON DELETE CASCADE)"))
    {
        qWarning() << create.lastError().text();
    }
#endif // FAVORITE
    if (!create.exec("SELECT name FROM sqlite_master WHERE type='table';")) qWarning() << create.lastError().text();
    while (create.next())
    {
        QString tablename = create.value(0).toString();
        QSqlQuery getColumns(db);
        if (!getColumns.exec(QString("PRAGMA table_info(%1);").arg(tablename))) qWarning() << getColumns.lastError().text();
        QMap<QString, int> columns;
        while (getColumns.next()) columns.insert(getColumns.value(1).toString(), getColumns.value(0).toInt());
        header.insert(tablename, columns);
    }
}