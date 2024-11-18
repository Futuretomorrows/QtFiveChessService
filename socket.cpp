#include "socket.h"

Socket::Socket():index(-1)
{

}
Socket::Socket(QTcpSocket* p){
    this->client=p;
    this->index=-1;
}
Socket::operator QTcpSocket*(){
    return this->client;
}
