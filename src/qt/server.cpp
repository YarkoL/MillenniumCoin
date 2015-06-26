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

/// BodyData

QString key = QString::fromUtf8("keyAbcdefghijklmnopqrstuvxyz");
bool file_is_encrypted;
QFile file("../xml/test.xml");
Aesqt aes;

Server::Server()
{
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
        this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    if (!file_is_encrypted) {
        encryptFile(file);
       // file_is_encrypted = true;
    }
    server->listen(QHostAddress::Any, 8080);
}

void Server::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    Q_UNUSED(req);


    file.open( QFile::ReadOnly);
    QByteArray body = file.exists() ? file.readAll() : "<heading>not found</heading>";

    resp->setHeader("Content-Type", "text/xml");
    resp->setHeader("Content-Length", QString::number(body.size()));
    resp->writeHead(200);
    file.close();
    resp->end(body);
}

bool Server::encryptFile(QFile &f) {
    //change to qstring
    QString str;
    if (!f.open(QFile::ReadOnly | QFile::Text)) return false;
    QTextStream in(&f);
    str = in.readAll();
    qDebug() << "qstring" << str;
    f.close();

    QString ciphertxt = aes.encodeText(str, key);
    qDebug() << "ciphertxt" << ciphertxt;

    QString plaintxt = aes.decodeText(ciphertxt, key);
    qDebug() << "plaintxt" << plaintxt;
    return false;
}

QString Server::decryptFile(QFile &f) {
    return "";
}
/*TODO use AES to encrypt file and decrypt on server
 some pointers:
 http://www.essentialunix.org/index.php?option=com_content&view=article&id=48:qcatutorial&catid=34:qttutorials&Itemid=53
 https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

*/
