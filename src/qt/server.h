
#ifndef SERVER_H
#define SERVER_H


#include "qhttpserver/qhttpserverfwd.h"

#include <QObject>
#include <QFile>

/// BodyData

class Server : public QObject
{
    Q_OBJECT

    bool encryptFile(QFile &f);
    QString decryptFile(QFile &f);
public:
    Server();

private slots:
    void handleRequest(QHttpRequest *req, QHttpResponse *resp);
};

#endif // SERVER_H
