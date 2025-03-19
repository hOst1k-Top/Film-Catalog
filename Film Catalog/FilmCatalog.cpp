#include "include/FilmCatalog.h"

FilmCatalog::FilmCatalog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FilmCatalogClass())
{
    ui->setupUi(this);
    if (!UserProvider::getInstance()->getAdmin()) ui->adminmenu->hide();
    QObject::connect(ui->adminmenu, &QAbstractButton::clicked, this, [](){
        AdminMenu* menu = new AdminMenu();
        menu->show();
        });
    QObject::connect(ui->selfedit, &QAbstractButton::clicked, this, [&](){
        UserSelfEdit* edit = new UserSelfEdit();
        QObject::connect(edit, &UserSelfEdit::showAuth, this, &FilmCatalog::onAuthRequested);
        edit->show();
        });
    QObject::connect(ui->display, &QListWidget::itemDoubleClicked, [&](QListWidgetItem* item) {
        if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) && UserProvider::getInstance()->getAdmin())
        {
            emit RequestDeleteByID(item->data(Qt::UserRole).toInt());
            ui->display->removeItemWidget(item);
            delete item;
        }
        else if(QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier) && UserProvider::getInstance()->getAdmin())
        {
            // TODO: Edit film menu
        }
        else
        {
            DisplayCatalog* display = new DisplayCatalog(item->data(Qt::UserRole).toInt());
            display->show();
        }
        });
    QObject::connect(ui->add, &QAbstractButton::clicked, [this]() {
        CatalogItemAdder* adder = new CatalogItemAdder();
        QObject::connect(adder, &CatalogItemAdder::requestAdding, this, &FilmCatalog::onAddRequested);
        if (adder->exec())
        {
            return;
        }
        adder->deleteLater();
    });
    SelectItems();
}

void FilmCatalog::onAuthRequested()
{
    close();
    Authorization* auth = new Authorization();
    auth->show();
}

FilmCatalog::~FilmCatalog()
{
    delete ui;
}

void FilmCatalog::onAddRequested(int id)
{
    QListWidgetItem* item = new QListWidgetItem(ui->display);
    CatalogDisplayItem* card = new CatalogDisplayItem(id);
    QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
    QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
    item->setData(Qt::UserRole, id);
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setSizeHint(card->minimumSizeHint());
    ui->display->addItem(item);
    ui->display->setItemWidget(item, card);
}

void FilmCatalog::SelectItems()
{
    QSqlQuery query(DBProvider::getInstance()->getDB());
    query.prepare("SELECT id FROM Films");
    if (!query.exec()) qWarning() << query.lastError().text();
    while (query.next())
    {
        int id = query.value(0).toInt();
        QListWidgetItem* item = new QListWidgetItem(ui->display);
        CatalogDisplayItem* card = new CatalogDisplayItem(id);
        QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
        QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
        item->setData(Qt::UserRole, id);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setSizeHint(card->minimumSizeHint());
        ui->display->addItem(item);
        ui->display->setItemWidget(item, card);
    }
}
