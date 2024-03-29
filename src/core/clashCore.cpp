#include "clashCore.h"
#include "configurator.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QApplication>

ClashCore::ClashCore()
{
#ifdef Q_OS_WIN64
    clashFilePath = "clash-windows-amd64.exe";
#elif defined(Q_OS_WIN32)
    clashFilePath = "clash-windows-386.exe"
#elif defined(Q_OS_LINUX)
    clashFilePath = "clash";
#elif defined(Q_OS_MAC)
    clashFilePath = "/usr/local/bin/clash";
#endif

    QString clashConfigPath = Configurator::getClashConfigPath();
    QDir dir(clashConfigPath);
    if (!dir.exists()) {
        dir.mkpath(clashConfigPath);
    }
    QString appPath = QApplication::applicationDirPath();
    if (!QFile::exists(clashConfigPath + "config.yaml"))
        QFile::copy(appPath + "/config/clash.yaml", clashConfigPath + "config.yaml");
    if (!QFile::exists(clashConfigPath + "Country.mmdb"))
        QFile::copy(appPath + "/config/Country.mmdb", clashConfigPath + "Country.mmdb");

    clashProcess = new QProcess;
}

ClashCore& ClashCore::instance()
{
    static ClashCore clashManagerInstance;
    return clashManagerInstance;
}

ClashCore::~ClashCore()
{
    this->stop();
    delete clashProcess;
}

bool ClashCore::isRunning()
{
    return clashProcess->state() == QProcess::Running;
}

bool ClashCore::start(const QString& configFilePath)
{
    QStringList arguments;
    if (!configFilePath.isEmpty())
        arguments << "-f" << configFilePath;
    clashProcess->start(clashFilePath, arguments);
    clashProcess->waitForFinished(500);
    int exitCode = clashProcess->exitCode();
    if (exitCode != 0) {
        qCritical() << "Failed to start clash!";
    }

    return exitCode == 0;
}

bool ClashCore::stop()
{
    clashProcess->kill();
    clashProcess->waitForFinished();
    return clashProcess->state() == QProcess::NotRunning;
}

bool ClashCore::restart(const QString& configFilePath)
{
    stop();
    start(configFilePath);
    return isRunning();
}
