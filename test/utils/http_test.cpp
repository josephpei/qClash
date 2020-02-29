#include "src/utils/httputil.h"

#include <QtTest>
#include <QUrl>
#include <QByteArray>
#include <QJsonDocument>

class HttpTest : public QObject {
    Q_OBJECT

private slots:
    void testGet();
};

void HttpTest::testGet()
{
    QByteArray res = HttpUtil::instance().get(QUrl("http://httpbin.org/get"));
    QJsonDocument doc = QJsonDocument::fromJson(res);

    QVERIFY(doc["url"].toString() == "http://httpbin.org/get");
}

QTEST_GUILESS_MAIN(HttpTest)
#include "http_test.moc"