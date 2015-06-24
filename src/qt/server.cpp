#include "server.h"

#include <QCoreApplication>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

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
    new Responder(req, resp);
}

/// Responder

Responder::Responder(QHttpRequest *req, QHttpResponse *resp)
    : m_req(req)
    , m_resp(resp)
{

    resp->setHeader("Content-Type", "text/xml");
    resp->writeHead(200);

    QString bodyStart = tr("<note><to>Tove</to><from>Jani</from><heading>Reminder</heading><body>Don't forget me this weekend!</body>");
    resp->write(bodyStart.toUtf8());

    connect(req, SIGNAL(data(const QByteArray&)), this, SLOT(accumulate(const QByteArray&)));
    connect(req, SIGNAL(end()), this, SLOT(reply()));
    connect(m_resp, SIGNAL(done()), this, SLOT(deleteLater()));
}

Responder::~Responder()
{
}

void Responder::accumulate(const QByteArray &data)
{
    m_resp->write(data);
}

void Responder::reply()
{
    m_resp->end(QByteArray("</note>"));
}
