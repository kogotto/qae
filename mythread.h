#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>
#include <QTime>


class Worker:
        public QObject
{
    Q_OBJECT

public slots:
    void doWork();

signals:
    void sendResults(QTime time, int result);
};


#endif // MYTHREAD_H
