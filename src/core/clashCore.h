#pragma once

#include <QProcess>

class ClashCore : public QObject {
    Q_OBJECT
public:
    static ClashCore &instance();
    ClashCore(const ClashCore &) = delete;
    void operator=(const ClashCore &) = delete;
    ~ClashCore();

    QString getVersion();
    bool isRunning();
    bool start();
    bool stop();
    bool restart();

private:
    ClashCore();
    QProcess *clashProcess;
    QString clashFilePath;
};