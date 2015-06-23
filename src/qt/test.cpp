#include "test.h"
#include "ui_test.h"


 Test::Test(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Test)
{
    ui->setupUi(this);
    ui->heading->setText("test XML");
}

void Test::GetUrl()
{
    qDebug() << "Connecting";

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
    manager->get(QNetworkRequest(QUrl("http://www.w3schools.com/xml/note.xml")));

}

void Test::replyFinished(QNetworkReply *reply)
{
    QByteArray data;
    qDebug() << "Response..";
    if(reply->isOpen()) {
        data = reply->readAll();
        //qDebug() << data;
        reply->close();
    }
    parseXml(data);
}

void Test::parseXml(const QByteArray & data)
{
    QXmlStreamReader xml(data);

    //Reading from the file
    /* We'll parse the XML until we reach end of it.*/
    while (!xml.atEnd() && !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();

        /* If token is just StartDocument, we'll go to next.*/
        if (token == QXmlStreamReader::StartDocument)
            continue;

        /* If token is StartElement, we'll see if we can read it.*/
        if (token == QXmlStreamReader::StartElement) {
            if  (xml.name() == "to") {
                xml.readNext();
                ui->recipient->setText(xml.text().toString());
            }
            else if (xml.name() == "from") {
                xml.readNext();
                ui->sender->setText(xml.text().toString());
            }
            else if  (xml.name() == "heading") {
                xml.readNext();
                ui->heading->setText(xml.text().toString());
            }
            else if  (xml.name() == "body") {
                xml.readNext();
                ui->body->setText(xml.text().toString());
            }
        }
    }

    /* Error handling. */
    if (xml.hasError()) {
        ui->body->setText(xml.errorString());
    }
}

