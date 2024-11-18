#include "mytask.h"
mytask::mytask(std::shared_ptr<Tcp>tasks):task(tasks){
   this->setAutoDelete(true);
}
mytask::mytask(){
    this->setAutoDelete(true);
}
void mytask::run(){
    this->task->match();
}
void mytask::setTask(std::shared_ptr<Tcp>tasks){
    this->task=tasks;
}
