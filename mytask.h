#ifndef THREAD_H
#define THREAD_H

#include<QRunnable>
#include"tcp.h"
class mytask:public QRunnable{
public:
    mytask();
    mytask(std::shared_ptr<Tcp>tasks);
    void setTask(std::shared_ptr<Tcp>tasks);
    void run() override;
    std::shared_ptr<Tcp>task;
};


#endif // THREAD_H
