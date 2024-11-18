#include "tcp.h"
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Tcp w;
    std::cout<<"main";
    return a.exec();
}
