#include "server.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <aesqt.h>

#include <qhttpserver/qhttpserver.h>
#include <qhttpserver/qhttprequest.h>
#include <qhttpserver/qhttpresponse.h>

QString key = QString::fromUtf8("keyAbcdefghijklmnopqrstuvxyz"); //TODO key generation
bool file_is_encrypted = false;
QFile file("../xml/test.xml");
Aesqt aes;

Server::Server()
{
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
        this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    if (!file_is_encrypted) {
        encryptFile(file);
        file_is_encrypted = true;
    }
    server->listen(QHostAddress::Any, 8080);
}

void Server::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    Q_UNUSED(req);

    QByteArray body = file_is_encrypted ? decryptFile(file).toUtf8() : file.readAll();

    resp->setHeader("Content-Type", "text/xml");
    resp->setHeader("Content-Length", QString::number(body.size()));
    resp->writeHead(200);
    file.close();
    resp->end(body);
}

bool Server::encryptFile(QFile &f) {

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&f); //put unencrypted file in

    QString str = in.readAll(); //and convert to a string

    //qDebug() << "qstring" << str;

    //encrypt
    QString ciphertxt = aes.encodeText(str, key);
    //qDebug() << "ciphertxt" << ciphertxt;
    f.close();

    //overwrite file with encrypted content
    if (!f.open(QIODevice::WriteOnly| QIODevice::Truncate | QIODevice::Text))
            return false;
    in << ciphertxt;
    f.flush();
    f.close();

    return true;
}

QString Server::decryptFile(QFile &f) {
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return "<heading>Failed decrypting</heading>";
    QTextStream in(&f); //put encrypted file in
    QString str = in.readAll(); //and convert to a string
    f.close();
    //qDebug() << "qstring" << str;

    //decrypt
    QString plaintxt = aes.decodeText(str, key);
    //qDebug() << "plaintxt" << plaintxt;

    return plaintxt;
}
