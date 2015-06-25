#include "server.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <openssl/aes.h>

#include <qhttpserver/qhttpserver.h>
#include <qhttpserver/qhttprequest.h>
#include <qhttpserver/qhttpresponse.h>

/// BodyData

Server::Server()
{
    QHttpServer *server = new QHttpServer(this);
    connect(server, SIGNAL(newRequest(QHttpRequest*, QHttpResponse*)),
        this, SLOT(handleRequest(QHttpRequest*, QHttpResponse*)));

    server->listen(QHostAddress::Any, 8080);
}

void Server::handleRequest(QHttpRequest *req, QHttpResponse *resp)
{
    Q_UNUSED(req);

    QFile file("../xml/test.xml");
    file.open( QFile::ReadOnly);
    QByteArray body = file.exists() ? file.readAll() : "<heading>not found</heading>";

    resp->setHeader("Content-Type", "text/xml");
    resp->setHeader("Content-Length", QString::number(body.size()));
    resp->writeHead(200);
    resp->end(body);
}

/*TODO use AES to encrypt file and decrypt on server
 some pointers:
 http://www.essentialunix.org/index.php?option=com_content&view=article&id=48:qcatutorial&catid=34:qttutorials&Itemid=53
 https://wiki.openssl.org/index.php/EVP_Symmetric_Encryption_and_Decryption

*/
