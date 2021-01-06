#include "BuildConfig.h"
#include "mainwindow.h"

#include <QApplication>
#include <singleapplication.h>
#include <QTranslator>
#include <QMetaType>
#include <QSettings>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QSettings::setDefaultFormat(QSettings::NativeFormat);
    QApplication::setOrganizationName("qClash");
    QApplication::setApplicationName("qClash");

    qRegisterMetaTypeStreamOperators<QList<QString>>("QList<QString>");
    SingleApplication app(argc, argv);

    app.setApplicationName(QStringLiteral("qClash"));
    app.setApplicationVersion(QStringLiteral(QCLASH_VERSION));
    app.setQuitOnLastWindowClosed(false);

    QTranslator translator;
    QString langPath = QApplication::applicationDirPath() + "/lang";
    translator.load(QLocale::system().name(), langPath);
    app.installTranslator(&translator);
    MainWindow w;
    w.show();
    QObject::connect(&app, &SingleApplication::instanceStarted, &w, &QMainWindow::raise);
    return app.exec();
}
