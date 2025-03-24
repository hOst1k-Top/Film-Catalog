#pragma once

#include <QtWidgets/QWidget>
#include "UserProvider.h"
#include "dbprovider.h"
#include "AdminMenu.h"
#include "UserSelfEdit.h"
#include "Authorization.h"
#include "CatalogDisplayItem.h"
#include "DisplayCatalog.h"
#include "CatalogItemAdder.h"
#include "../ui/ui_FilmCatalog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FilmCatalogClass; };
QT_END_NAMESPACE

struct Movie
{
    int id;
    QString title;
    QString release;
    QStringList genres;
};

class FilmCatalog : public QWidget
{
    Q_OBJECT

public:
    FilmCatalog(QWidget *parent = nullptr);
    ~FilmCatalog();

public slots:
    void onAuthRequested();
    void onAddRequested(int id, QString title, QString release, QStringList genres);
    void filter();

signals:
    void RequestUpdateDisplay();
    void RequestDeleteByID(int id);

private:
    void SelectItems();
    void GetFilters();
    void addItemToPage(int id, const QString& title, QString release, const QStringList& genres);
#ifdef FAVORITE
    QList<int> favoriteList;
#endif // FAVORITE
    QVector<QList<Movie>> pages;
    Ui::FilmCatalogClass *ui;
    const int PAGE_SIZE = 10;
};
