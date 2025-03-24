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
#if defined(FAVORITE) && defined(SOCIAL) && defined(AUTHORIZATION)
        else if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) && UserProvider::getInstance()->getAdmin())
        {
            QSqlQuery query(DBProvider::getInstance()->getDB());
            if (item->data(Qt::UserRole + 4).toBool())
            {
                favoriteList.removeOne(item->data(Qt::UserRole).toInt());
                item->setData(Qt::UserRole + 4, false);
                query.prepare("DELETE FROM Favorite WHERE user = :usr AND favfilm = :id");
                query.bindValue(":usr", UserProvider::getInstance()->getLogin());
                query.bindValue(":id", item->data(Qt::UserRole).toInt());
                if (!query.exec()) qWarning() << query.lastError().text();
                emit RequestUpdateDisplay();
            }
            else
            {
                favoriteList.append(item->data(Qt::UserRole).toInt());
                item->setData(Qt::UserRole + 4, true);
                query.prepare("INSERT INTO Favorite(favfilm, user) VALUES (:id,:usr)");
                query.bindValue(":usr", UserProvider::getInstance()->getLogin());
                query.bindValue(":id", item->data(Qt::UserRole).toInt());
                if (!query.exec()) qWarning() << query.lastError().text();
                emit RequestUpdateDisplay();
            }
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
#ifdef FILTER
    QObject::connect(ui->filter, &QLineEdit::textChanged, this, &FilmCatalog::filter);
#ifdef ADVANCEDFILTER
    ui->releaseFilter->addItem(tr("All"));
    ui->genreFilter->addItem(tr("All"));
#endif // ADVANCEDFILTER
#endif // FILTER
    QObject::connect(ui->currentPage, &QSpinBox::valueChanged, this, [&](int index) {
        while (ui->display->count())
        {
            QListWidgetItem* itm = ui->display->takeItem(0);
            ui->display->removeItemWidget(itm);
            delete itm;
        }
    
        for(auto movie : pages[index - 1])
        {
            QListWidgetItem* item = new QListWidgetItem(ui->display);
            CatalogDisplayItem* card = new CatalogDisplayItem(movie.id);
            QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
            QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
            item->setData(Qt::UserRole, movie.id);
            item->setData(Qt::UserRole + 1, movie.title);
            item->setData(Qt::UserRole + 2, movie.release);
            item->setData(Qt::UserRole + 3, movie.genres);
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            item->setSizeHint(card->minimumSizeHint());
            ui->display->addItem(item);
            ui->display->setItemWidget(item, card);
        }

        filter();
    });
#if !defined(SOCIAL) && !defined(AUTHORIZATION)
    ui->adminmenu->hide();
    ui->selfedit->hide();
#endif // !SOCIAL
#ifndef FILTER
    ui->filter->hide();
#ifndef ADVANCEDFILTER
    ui->releaseFilter->hide();
    ui->genreFilter->hide();
#endif // !ADVANCEDFILTER
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
#if defined(FAVORITE) && defined(SOCIAL) && defined(AUTHORIZATION)
    QSqlQuery favorite(DBProvider::getInstance()->getDB());
    favorite.prepare("SELECT favfilm FROM Favorite WHERE user = :usr");
    favorite.bindValue(":usr", UserProvider::getInstance()->getLogin());
    if (!favorite.exec()) qWarning() << favorite.lastError().text();
    while (favorite.next()) favoriteList.append(favorite.value(0).toInt());
#endif
    SelectItems();
#if defined(FILTER) && defined(ADVANCEDFILTER)
    QObject::connect(ui->releaseFilter, &QComboBox::currentTextChanged, this, &FilmCatalog::filter);
    QObject::connect(ui->genreFilter, &QComboBox::currentTextChanged, this, &FilmCatalog::filter);
#endif // FILTER
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

void FilmCatalog::onAddRequested(int id, QString title, QString release, QStringList genres)
{
#ifdef ADVANCEDFILTER
    GetFilters();
#endif // ADVANCEDFILTER
    QListWidgetItem* item = new QListWidgetItem(ui->display);
    CatalogDisplayItem* card = new CatalogDisplayItem(id);
    QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
    QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
    item->setData(Qt::UserRole, id);
    item->setData(Qt::UserRole + 1, title);
    item->setData(Qt::UserRole + 2, release);
    item->setData(Qt::UserRole + 3, genres);
#ifdef FAVORITE
    item->setData(Qt::UserRole + 4, false);
#endif // FAVORITE
    item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
    item->setSizeHint(card->minimumSizeHint());
    ui->display->addItem(item);
    ui->display->setItemWidget(item, card);
}

void FilmCatalog::filter()
{
    QString filter = ui->filter->text();
    QString release = ui->releaseFilter->currentText();
    QString genre = ui->genreFilter->currentText();
    if (filter.isEmpty() && release == tr("All") && genre == tr("All"))
    {
        for (int row = 0; row < ui->display->count(); ++row)
        {
            ui->display->item(row)->setHidden(0);
        }
        return;
    }
    for (int row = 0; row < ui->display->count(); ++row)
    {
        auto rowItem = ui->display->item(row);
        bool hide = !rowItem->data(Qt::UserRole + 1).toString().contains(filter, Qt::CaseInsensitive);
        hide = hide || (rowItem->data(Qt::UserRole + 2).toString() != release && release != "All");
        hide = hide || (rowItem->data(Qt::UserRole + 3).toString().contains(genre, Qt::CaseInsensitive) && genre != "All");
        rowItem->setHidden(hide);
    }
}

void FilmCatalog::SelectItems()
{
    QSqlQuery query(DBProvider::getInstance()->getDB());
    query.prepare("SELECT id,Title,Release,Genres FROM Films");
    if (!query.exec()) qWarning() << query.lastError().text();
    while (query.next()) addItemToPage(query.value(0).toInt(), query.value(1).toString(),query.value(2).toString(), query.value(3).toString().split(",", Qt::SkipEmptyParts));
    GetFilters();
#ifdef PAGES
    for (auto movie : pages[0])   
#else
    for (auto page : pages)
    {
        for(auto movie : page)
#endif // PAGES
        {
            QListWidgetItem* item = new QListWidgetItem(ui->display);
            CatalogDisplayItem* card = new CatalogDisplayItem(movie.id);
            QObject::connect(this, &FilmCatalog::RequestUpdateDisplay, card, &CatalogDisplayItem::UpdateDisplay);
            QObject::connect(this, &FilmCatalog::RequestDeleteByID, card, &CatalogDisplayItem::RequestDelete);
            item->setData(Qt::UserRole, movie.id);
            item->setData(Qt::UserRole + 1, movie.title);
            item->setData(Qt::UserRole + 2, movie.release);
            item->setData(Qt::UserRole + 3, movie.genres);
#ifdef FAVORITE
            item->setData(Qt::UserRole + 4, favoriteList.contains(movie.id));
#endif
            item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
            item->setSizeHint(card->minimumSizeHint());
            ui->display->addItem(item);
            ui->display->setItemWidget(item, card);
        }
#ifndef PAGES
    }
#endif // !PAGES

}

void FilmCatalog::GetFilters()
{
    ui->releaseFilter->clear();
    ui->genreFilter->clear();
    ui->releaseFilter->addItem(tr("All"));
    ui->genreFilter->addItem(tr("All"));
    QString queryText = R"(
        WITH RECURSIVE split(genre, rest) AS (
            SELECT 
                TRIM(SUBSTR(Genres, 1, INSTR(Genres || ',', ',') - 1)) AS genre,
                CASE 
                    WHEN INSTR(Genres, ',') > 0 THEN SUBSTR(Genres, INSTR(Genres, ',') + 1)
                    ELSE ''
                END AS rest
            FROM Films
            WHERE Genres IS NOT NULL AND Genres <> ''

            UNION ALL

            SELECT 
                TRIM(SUBSTR(rest, 1, INSTR(rest || ',', ',') - 1)),
                CASE 
                    WHEN INSTR(rest, ',') > 0 THEN SUBSTR(rest, INSTR(rest, ',') + 1)
                    ELSE ''
                END
            FROM split
            WHERE rest <> ''
        )
        SELECT DISTINCT genre 
        FROM split
        WHERE genre <> ''
        ORDER BY genre;
    )";
    QSqlQuery getter(DBProvider::getInstance()->getDB());
    if (!getter.exec(queryText))
    {
        qWarning() << getter.lastError().text();
        return;
    }
    while (getter.next())
    {
        ui->genreFilter->addItem(getter.value(0).toString());
    }
    queryText = "SELECT DISTINCT Release FROM Films";
    if (!getter.exec(queryText))
    {
        qWarning() << getter.lastError().text();
        return;
    }
    while (getter.next())
    {
        ui->releaseFilter->addItem(getter.value(0).toString());
    }
}

void FilmCatalog::addItemToPage(int id, const QString& title, QString release, const QStringList& genres)
{
    if (pages.isEmpty() || pages.last().size() >= PAGE_SIZE) {
        pages.append(QList<Movie>());
    }
    pages.last().append({ id, title, release, genres });
    ui->currentPage->setMaximum(pages.length());
}
