#ifndef TCP_H
#define TCP_H

#include <QCoreApplication>
#include<QTcpServer>
#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlError>
#include<QtSql/QSqlQuery>
#include<QtSql/QSqlRecord>
#include<QQueue>
#include"DataStruct.h"
#include"socket.h"
#include<unordered_map>
#include<QObject>
#include<iostream>
class Tcp  : public QObject
{
    Q_OBJECT
public:
    Tcp();
    ~Tcp();
    bool findIfData(struct account accountes,QTcpSocket *client,qint8 type=2);
    QSqlDatabase data;
    QSqlQuery query;
    QTcpServer *sever;
    QQueue<QTcpSocket*> sock;
    QVector<Socket> matchVector;
    pointData IntData;
    int ik=0;
    QString f=0;
public slots:
    void del();
    void start();
    void newComing();
    void reaclient();
    void Zhuce(struct account,QTcpSocket*client);
    void matchComunication(Socket*first,Socket *Second);
    void match();
    void matchComunication2(Socket*p1,Socket *p2);
private:
};
#endif // TCP_H
