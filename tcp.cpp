#include "tcp.h"
#include <QRandomGenerator>
#include<mutex>
#include<functional>
#include<QThreadPool>
#include<QMutex>
#include"mytask.h"
QDataStream& operator<<(QDataStream& out, const account &p){
    out<<p.type<<p.accounts<<p.password;
    return out;
}
QDataStream& operator>>(QDataStream& in, account p){
    in>>p.type>>p.accounts>>p.password;
    return in;
}
QDataStream& operator<<(QDataStream& out, const ifHave &p){
    out<<p.type<<p.have;
    return out;
}
QDataStream& operator>>(QDataStream& in, ifHave &p){
    in>>p.type>>p.have;
    return in;
}
QDataStream& operator>>(QDataStream& in, pointData &p){
    in>>p.x>>p.y>>p.color;
    return in;
}
QDataStream& operator<<(QDataStream& out, pointData &p){
    out<<p.x<<p.y<<p.color;
    return out;
}
Tcp::Tcp() : QObject(NULL)
{

    ik=1;
//
    this->data=QSqlDatabase::addDatabase("QMYSQL");
    data.setHostName("localhost");
    data.setUserName("root");
    data.setPassword("root");
    data.setDatabaseName("Five_Chess");
    query=QSqlQuery(data);
    if(!data.open()){
        std::cout<<"无法正常打开数据库"<<data.lastError().databaseText().toStdString()<<std::endl;
    }
    else{
        std::cout<<"it is well to open the SQL"<<std::endl;
//        query.prepare("select * from users ");//话说这是干嘛的
    }
    start();
}
void Tcp::start(){//外部主机链接得将防火墙关了
    this->sever=new QTcpServer;
    if(!sever->listen(QHostAddress::Any,9091)){//如果端口为0通常认为是无效的
        std::cout<<sever->errorString().toStdString();
        return ;
    }
    else
    {
        //QString p("makeba");
        std::string p("服务器启动成功");
        std::cout<<p<<std::endl;
    }
    connect(this->sever,&QTcpServer::newConnection,this,&Tcp::newComing);
}
void Tcp::newComing(){
    QTcpSocket *client=new QTcpSocket;
    f+="1";
    std::cout<<f.toStdString()<<std::endl;
    //qDebug()<<client;
    client= this->sever->nextPendingConnection();
    this->sock.push_back(client);
    connect(client,&QTcpSocket::readyRead,this,&Tcp::reaclient);
    connect(client,&QTcpSocket::disconnected,this,&Tcp::del);
}
void Tcp::reaclient(){//实现通讯
    QTcpSocket *client=static_cast<QTcpSocket*>(sender());
    qint8 type=55;
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_14);
//    qint32 b;
    while (client->bytesAvailable()) {
//        qDebug()<<client->bytesAvailable();
//          if (client->bytesAvailable() < (int)sizeof(qint32)) {
//              // 数据不完整，等待更多数据
//              qDebug()<<"1";
//              break;
//          }
//    if(ik==1){
//    in>>b;
//    ik=0;
//    }
//    qDebug()<<"b"<<b<<"  "<<(int)b-sizeof(qint32)<<"  "<<(int)client->bytesAvailable();
//          if ((int)client->bytesAvailable() < (int)b-sizeof(qint32)) {
//              qDebug()<<"2error";
//              //reaclient();
//              QThread::sleep(1);
//              break;
//          }
    in>>type;
    std::cout<<"type"<<" "<<type<<std::endl;
    switch (type) {
    case 1:{//注册
        struct account accountes;
        QString account;
        QString password;
        in>>account;
        in>>password;
        accountes.accounts=account;
        accountes.password=password;
        Zhuce(accountes,client);
        break;
    }
    case 2:{

        break;
    }
    case 3:{
        struct account accountes;
        QString account;
        QString password;
        in>>account;
        in>>password;
        accountes.accounts=account;
        accountes.password=password;
        {
            struct ifHave haves;
            if(findIfData(accountes,client,3))
            haves.have=true;
            else  haves.have=false;
            haves.type=3;//通知登录页面
            QByteArray block;
            QDataStream out(&block,QIODevice::WriteOnly);
            out.setVersion(QDataStream::Qt_5_14);
            out<<haves;
//            qDebug()<<haves.have;
//            b=(qint32)block.size();
//            out.device()->seek(0);
//            out<<b;
            client->write(block);
            client->flush();
        }
        break;
    }
    case 4:{//匹配信号
        int size=this->matchVector.size();
        bool A=false;
        for(int i=0;i<size;i++){
            if(matchVector[i].client==client){
                A=true;
            }
        }
        if(A)break;
        else
        this->matchVector.push_back(Socket(client));
        QByteArray P;
        QDataStream in(&P,QIODevice::WriteOnly);
        in.setVersion(QDataStream::Qt_5_14);
        in<<6;
        client->write(P);
        client->flush();
 //       std::shared_ptr<Tcp> task(this);// 他奶奶的，这个智能指针当这个大括号结束就自动西沟于是这个
        //this示例就毁了
 //       mytask *Task= new mytask;
 //       qDebug()<<"ssf";
//        Task->setTask(task);
//        QThreadPool::globalInstance()->start(Task);
        //qDebug()<<"match later";
        this->match();
        break;
    }
    case 5:{//位置信息
       pointData point;
       QDataStream in(client);
       break;
    }
  }
 }
}
void Tcp::del(){
    qDebug()<<"del is right";
    QTcpSocket *client=static_cast<QTcpSocket*>(sender());
    this->sock.removeOne(client);
    delete client;
}
void Tcp::Zhuce(struct account accountes,QTcpSocket *client){
     struct ifHave haves;
     haves.type=2;//通知注册页面已经有人了
            if(findIfData(accountes,client,2)){
                haves.have=true;
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_14);
                out<<haves;
                client->write(block);
                client->flush();
                return;
            }
            query.prepare("insert into users (account,password,star)"
                          "values(:account,:password,0)");
            query.bindValue(":account",accountes.accounts);
            query.bindValue(":password",accountes.password);
            if(query.exec()){
                qDebug()<<haves.type;
                haves.have=false;
                QByteArray block;
                QDataStream out(&block,QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_5_14);
                out<<haves;
                client->write(block);
                client->flush();
                return ;
            }
            else{
               std::cout<<"数据库执行错误"<<std::endl;
                return ;
            }
}
bool Tcp::findIfData(struct account accountes,QTcpSocket *client,qint8 type){
    query.prepare("select account from users where account = :acc ");
    query.bindValue(":acc",accountes.accounts);
    query.exec();
    if(query.next()){

       return true;
    }

    return false;
}
void Tcp::match(){
    {
       // qDebug()<<"match is good";
        qint8 type=6;
        QByteArray R;
        QDataStream out(&R,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_14);
        out<<type;//正在匹配
        std::cout<<"size "<<this->matchVector.size()<<std::endl;
        if(this->matchVector.size()<=1)
        {
//              this->matchVector.at(0).client->write(R);
//              this->matchVector.at(0).client->flush();
            Socket *p100=&(matchVector[0]);
            p100->client->write(R);
            p100->client->flush();
            std::cout<<"return is good"<<std::endl;
            return;
        }
        int first = 0;//QRandomGenerator::global()->bounded(0, this->matchVector.size());
        A:int second=1;// QRandomGenerator::global()->bounded(0, this->matchVector.size());
        if(second==first)goto A;
        //qDebug()<<"second";
        Socket *p1=new Socket(this->matchVector.at(first));
        Socket *p2=new Socket(this->matchVector.at(second));
        //qDebug()<<"first";
        this->matchVector.removeAt(first);
        this->matchVector.removeAt(first);
        std::cout<<"move"<<std::endl;
        p1->client->write(R);
        p2->client->write(R);
        p1->client->flush();
        p2->client->flush();
        p1->other=p2;
        p2->other=p1;
//        QByteArray byte;
//        QDataStream iss(&byte,QIODevice::WriteOnly);
//        iss.setVersion(QDataStream::Qt_5_14);
//        qint8 l=5;
//        iss<<l;
//        p1->client->write(byte);
//        p1->client->flush();
        this->matchComunication(p1,p2);
        //m.unlock();
    }
}
void  Tcp::matchComunication(Socket*p1,Socket *p2){
    int j=(QRandomGenerator::global()->bounded(1000)%2);
    QByteArray IntFirst;
    QDataStream IF(&IntFirst,QIODevice::WriteOnly);
    IF.setVersion(QDataStream::Qt_5_14);
    QByteArray IntSecond;
    QDataStream IS(&IntSecond,QIODevice::WriteOnly);
    IS.setVersion(QDataStream::Qt_5_14);
    qint8 chessColor=5;//确定两方的棋子类别
    int chessColorFirst=j;
    int chessColorSecond=1-j;
    IF<<chessColor<<chessColorFirst;
    qDebug()<<"firsr"<<chessColorFirst;
    qDebug()<<"second"<<chessColorSecond;
    p1->client->write(IntFirst);
    p1->client->flush();
    //qDebug()<<"send1 is good";
    IS<<chessColor<<chessColorSecond;
    p2->client->write(IntSecond);
    p2->client->flush();
    std::cout<<"send2 is good"<<std::endl;
    connect(p1->client,&QTcpSocket::readyRead,this,[=](){
        this->matchComunication2(p1,p2);
    });
    connect(p2->client,&QTcpSocket::readyRead,this,[=](){
        this->matchComunication2(p2,p1);
    });
}
void  Tcp::matchComunication2(Socket*p1,Socket *p2){
    QByteArray B;
    QDataStream in(p1->client);
    in.setVersion(QDataStream::Qt_5_14);
    qint8 type;
    in>>type;
    if(type==7){
        pointData point;
        in>>point;
        QByteArray A;
        QDataStream out(&A,QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_5_14);
        qint8 types=7;
        out<<types;
        out<<point;
        p2->client->write(A);
        p2->client->flush();
    }
}
Tcp::~Tcp()
{

}

