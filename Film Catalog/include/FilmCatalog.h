#pragma once

#include <QtWidgets/QWidget>
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

private:
    Ui::FilmCatalogClass *ui;
};
