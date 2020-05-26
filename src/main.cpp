#include "BuildConfig.h"
#include "mainwindow.h"

#include <QApplication>
#include <singleapplication.h>
#include <QTranslator>

int main(int argc, char *argv[])
{
    SingleApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("qClash"));
    app.setApplicationVersion(QStringLiteral(QCLASH_VERSION));

    QTranslator translator;
    translator.load(QLocale::system().name(), "./lang");
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    QObject::connect(&app, &SingleApplication::instanceStarted, &w, &QMainWindow::raise);
    return app.exec();
}
