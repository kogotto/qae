#include "mythread.h"

MyThread::MyThread()
{

}

void MyThread::run()
{
    while(1) {
        const auto time = qrand() % 2000 + 1000;
        const auto result = qrand() % 10;
        msleep(time);
        emit sendResults(QTime(0, 0).addMSecs(time), result);
    }
}

