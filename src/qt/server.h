
#ifndef SERVER_H
#define SERVER_H


#include "qhttpserver/qhttpserverfwd.h"

#include <QObject>
#include <QScopedPointer>

/// BodyData

class Server : public QObject
{
    Q_OBJECT

public:
    Server();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};

/// Responder

class Responder : public QObject
{
    Q_OBJECT

public:
    Responder(QHttpRequest *req, QHttpResponse *resp);
    ~Responder();

signals:
    void done();

private slots:
    void accumulate(const QByteArray &data);
    void reply();

private:
    QScopedPointer<QHttpRequest> m_req;
    QHttpResponse *m_resp;
};

#endif // SERVER_H
