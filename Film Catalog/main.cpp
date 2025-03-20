#include "include/Authorization.h"
#include "include/FilmCatalog.h"
#include <QtWidgets/QApplication>

/*
Options:
#define AUTHORIZATION - Enable authorization to the app
#define SOCIAL - Enable social function (admin menu and selfedit)
#define FILTER - Enable filter feature
#define PAGES - Enable part-loading list
#define BUCKET - Enable bucket feature
*/

#define AUTHORIZATION 
#define SOCIAL 
#define FILTER 
#define PAGES

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
