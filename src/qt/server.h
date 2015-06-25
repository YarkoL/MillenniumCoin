
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

#endif // SERVER_H
