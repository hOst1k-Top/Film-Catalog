#include "include/FilmCatalog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/icon.png"));
    FilmCatalog w;
    a.setStyle("fusion");
    w.show();
    return a.exec();
}
