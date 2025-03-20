#include "include/Authorization.h"
#include "include/FilmCatalog.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icon/icon.png"));
#ifdef AUTHORIZATION
    Authorization w;
#else
    FilmCatalog w;
#endif // AUTHORIZATION    
    a.setStyle("fusion");
    w.show();
    return a.exec();
}
