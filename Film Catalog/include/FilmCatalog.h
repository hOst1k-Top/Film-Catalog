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

class FilmCatalog : public QWidget
{
    Q_OBJECT

public:
    FilmCatalog(QWidget *parent = nullptr);
    ~FilmCatalog();

public slots:
    void onAuthRequested();
    void onAddRequested(int id, QString title);
    void filter(QString filter);

signals:
    void RequestUpdateDisplay();
    void RequestDeleteByID(int id);

private:
    void SelectItems();
    void addItemToPage(int id, QString title);
    QVector<QList<QPair<int, QString>>> pages;
    Ui::FilmCatalogClass *ui;
    const int PAGE_SIZE = 10;
};
