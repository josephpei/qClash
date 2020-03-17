#include "mainwindow.h"

#include <QApplication>
#include <singleapplication.h>
#include <QTranslator>

int main(int argc, char *argv[])
{
    SingleApplication a(argc, argv);
    QTranslator translator;
    translator.load(QLocale::system().name(), "./lang");
    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    QObject::connect(&a, &SingleApplication::instanceStarted, &w, &QMainWindow::raise);
    return a.exec();
}
