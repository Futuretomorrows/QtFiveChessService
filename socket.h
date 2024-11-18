#ifndef SOCKET_H
#define SOCKET_H

#include<QTcpSocket>
class Socket
{
public:
    Socket();
    Socket(QTcpSocket*p);
    operator QTcpSocket*();
    QTcpSocket *client;
    int index;
    Socket* other;
};

#endif // SOCKET_H
