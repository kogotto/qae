#include "mythread.h"


void Worker::doWork()
{
    for (size_t i = 0; i < 8; ++i) {
        const auto time = qrand() % 2000 + 1000;
        const auto result = qrand() % 10;
        QThread::msleep(time);
        emit sendResults(QTime(0, 0).addMSecs(time), result);
    }
}

