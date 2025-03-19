#pragma once

#include <QtWidgets/QWidget>
#include "UserProvider.h"
#include "dbprovider.h"
#include "AdminMenu.h"
#include "UserSelfEdit.h"
#include "Authorization.h"
#include "CatalogDisplayItem.h"
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
    void onAddRequested(int id);

signals:
    void RequestUpdateDisplay();
    void RequestDeleteByID(int id);

private:
    void SelectItems();
    Ui::FilmCatalogClass *ui;
    const int PageCount = 25;
};
