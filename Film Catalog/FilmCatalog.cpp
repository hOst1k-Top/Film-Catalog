#include "include/FilmCatalog.h"

FilmCatalog::FilmCatalog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilmCatalogClass())
{
    ui->setupUi(this);
}

FilmCatalog::~FilmCatalog()
{
    delete ui;
}
