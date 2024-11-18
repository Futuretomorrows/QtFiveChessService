#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#endif // DATASTRUCT_H
#include<QString>
#include <QDataStream>
#include<QString>
struct pointData{
    int type;
    int color;
    int x;
    int y;
};

struct account{
    account();
    account(QString p,QString w);
    QString accounts;
    QString password;
    qint8 type=1;
    friend QDataStream& operator<<(QDataStream& out, const QString &p);
    friend QDataStream& operator>>(QDataStream& in, const QString &p);
};
struct ifHave{
    qint8 type;
    bool have=false;
};
