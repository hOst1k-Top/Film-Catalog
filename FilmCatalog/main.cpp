#include "include/Authorization.h"
#include "include/FilmCatalog.h"
#include <QtWidgets/QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString translationFile = QCoreApplication::applicationDirPath() + "/translations/Catalog_" + QLocale::system().name();
    translator.load(translationFile);
    a.installTranslator(&translator);
    a.setStyle("fusion");
    a.setWindowIcon(QIcon(":/icon/icon.png"));
#ifdef AUTHORIZATION
    Authorization w;
#else
    FilmCatalog w;
#endif // AUTHORIZATION    
    w.show();
    return a.exec();
}
