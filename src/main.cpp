#include "BuildConfig.h"
#include "mainwindow.h"

#include <QApplication>
#include <singleapplication.h>
#include <QTranslator>
#include <QMetaType>
#include <QSettings>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef Q_OS_WIN
    QSettings::setDefaultFormat(QSettings::IniFormat);
#else
    QSettings::setDefaultFormat(QSettings::NativeFormat);
#endif
    QApplication::setOrganizationName("qClash");
    QApplication::setApplicationName("qClash");

    qRegisterMetaTypeStreamOperators<QList<QString>>("QList<QString>");
    SingleApplication app(argc, argv);

    QApplication::setApplicationName(QStringLiteral("qClash"));
    QApplication::setApplicationVersion(QStringLiteral(QCLASH_VERSION));
    QApplication::setQuitOnLastWindowClosed(false);

    QTranslator translator;
    QString langPath;
#if defined(Q_OS_MAC)
    QDir resDir = QApplication::applicationDirPath();
    if (resDir.dirName() == "MacOS") {
        resDir.cdUp();
        resDir.cd("Resources");
    }
    langPath = resDir.absolutePath() + "/lang";
#else
    langPath = QApplication::applicationDirPath() + "/lang";
#endif
    translator.load(QLocale::system().name(), langPath);
    QApplication::installTranslator(&translator);
    MainWindow w;
    w.show();
    QObject::connect(&app, &SingleApplication::instanceStarted, &w, &QMainWindow::raise);
    return app.exec();
}
