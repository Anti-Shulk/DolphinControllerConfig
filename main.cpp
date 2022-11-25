#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QDesktopWidget>

int main(int argc, char *argv[])
{

    QApplication app(argc, argv);
    double scaleFactor = (app.desktop()->logicalDpiX()/96.0)*100.0;
    int height = QApplication::desktop()->height();
    int width = QApplication::desktop()->width();

    QStringList args;
    for (int i = 1; i < argc; i++) {
        args.append(argv[i]);
    }

    QFontDatabase::addApplicationFont(":/fonts/Exo 2 Condensed");

    QFontDatabase::addApplicationFont(":/fonts/Exo 2 Semi Bold Condensed");


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "dolphinqt5_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }
    MainWindow window(scaleFactor, height, width, args);
    window.show();
    return app.exec();
}
