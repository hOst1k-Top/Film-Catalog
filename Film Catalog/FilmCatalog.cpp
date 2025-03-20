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
#if defined(SOCIAL) && defined(AUTHORIZATION)
        if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier) && UserProvider::getInstance()->getAdmin())
#else
        if (QApplication::keyboardModifiers().testFlag(Qt::ShiftModifier))
#endif // SOCIAL
        {
            int id = item->data(Qt::UserRole).toInt();
            emit RequestDeleteByID(id);
            bool findToDelete = false;
            for (int page = 0; page < pages.length(); ++page)
            {
                if (findToDelete == true) break;
                for (int title = 0; title < pages[page].length(); ++title)
                {
                    if (title == id)
                    {
                        pages[page].removeAt(title);
                        findToDelete = true;
                        break;
                    }
                }
            }
            ui->display->removeItemWidget(item);
            delete item;
        }
#if defined(SOCIAL) && defined(AUTHORIZATION)
        else if (QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier) && UserProvider::getInstance()->getAdmin())
#else
        else if (QApplication::keyboardModifiers().testFlag(Qt::KeyboardModifier::ControlModifier))
#endif // SOCIAL
        {
            // TODO: Edit film menu
        }
#ifdef FAVORITE
#if defined(SOCIAL) && defined(AUTHORIZATION)
        else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) && UserProvider::getInstance()->getAdmin())
#else
        else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier))
#endif // SOCIAL
        {
            // TODO: Fovorite 
        }
#endif // FAVORITE
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
    QObject::connect(ui->filter, &QLineEdit::textChanged, this, &FilmCatalog::filter);
    QObject::connect(ui->currentPage, &QSpinBox::valueChanged, this, [&](int index) {
        while (ui->display->count())
        {
            QListWidgetItem* itm = ui->display->takeItem(0);
            ui->display->removeItemWidget(itm);
            delete itm;
        }
        
        for(auto [id, title] : pages[index - 1])
        {
            QListWidgetItem* item = new QListWidgetItem(ui->display);
            CatalogDisplayItem* card = new CatalogDisplayItem(id);
            QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
            QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
            item->setData(Qt::UserRole, id);
            item->setData(Qt::UserRole + 1, title);
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            item->setSizeHint(card->minimumSizeHint());
            ui->display->addItem(item);
            ui->display->setItemWidget(item, card);
        }
    });
#if !defined(SOCIAL) && !defined(AUTHORIZATION)
    ui->adminmenu->hide();
    ui->selfedit->hide();
#endif // !SOCIAL
#ifndef FILTER
    ui->filter->hide();
#endif // !FILTER
#ifndef PAGES
    ui->currentPage->hide();
    ui->leftArrow->hide();
    ui->rightArrow->hide();
#endif // !PAGES
#ifdef AUTHORIZATION
    if (!UserProvider::getInstance()->getAdmin())
    {
        ui->add->hide();
    }
#endif // AUTHORIZATION
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

void FilmCatalog::onAddRequested(int id, QString title)
{
    QListWidgetItem* item = new QListWidgetItem(ui->display);
    CatalogDisplayItem* card = new CatalogDisplayItem(id);
    QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
    QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
    item->setData(Qt::UserRole, id);
    item->setData(Qt::UserRole + 1, "");
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setSizeHint(card->minimumSizeHint());
    ui->display->addItem(item);
    ui->display->setItemWidget(item, card);
}

void FilmCatalog::filter(QString filter)
{
    if (filter.isEmpty() && ui->filter->text().isEmpty())
    {
        for (int row = 0; row < ui->display->count(); ++row)
        {
            ui->display->item(row)->setHidden(0);
        }
        return;
    }
    for (int row = 0; row < ui->display->count(); ++row)
    {
        ui->display->item(row)->setHidden(!ui->display->item(row)->data(Qt::UserRole + 1).toString().contains(filter, Qt::CaseInsensitive));
    }
}

void FilmCatalog::SelectItems()
{
    QSqlQuery query(DBProvider::getInstance()->getDB());
    query.prepare("SELECT id,Title FROM Films");
    if (!query.exec()) qWarning() << query.lastError().text();
    while (query.next()) addItemToPage(query.value(0).toInt(), query.value(1).toString());
#ifdef PAGES
    for (auto [id, title] : pages[0])
    
#else
    for (auto page : pages)
    {
        for(auto [id, title] : page)
#endif // PAGES
        {
            QListWidgetItem* item = new QListWidgetItem(ui->display);
            CatalogDisplayItem* card = new CatalogDisplayItem(id);
            QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
            QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
            item->setData(Qt::UserRole, id);
            item->setData(Qt::UserRole + 1, title);
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            item->setSizeHint(card->minimumSizeHint());
            ui->display->addItem(item);
            ui->display->setItemWidget(item, card);
        }
#ifndef PAGES
    }
#endif // !PAGES

}

void FilmCatalog::addItemToPage(int id, QString title)
{
    if (pages.isEmpty() || pages.last().size() >= PAGE_SIZE) {
        pages.append(QList<QPair<int, QString>>());
    }
    pages.last().append(qMakePair(id, title));
    ui->currentPage->setMaximum(pages.length());
}
