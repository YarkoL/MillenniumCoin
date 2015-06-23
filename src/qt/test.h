#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include <QDebug>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QXmlStreamReader>

namespace Ui {
    class Test;
}
class Test : public QDialog
{
    Q_OBJECT
public:
    explicit Test(QWidget *parent = 0);
    void parseXml(const QByteArray &data);
private:
 Ui::Test *ui;
signals:

public slots:
    void GetUrl();
    void replyFinished(QNetworkReply* reply);
};

#endif // TEST_H
