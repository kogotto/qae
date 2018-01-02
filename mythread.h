#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>
#include <QTime>


class MyThread:
        public QThread
{
    Q_OBJECT

public:
    MyThread();

signals:
    void sendResults(QTime time, int result);

protected:
    void run() override;
};

#endif // MYTHREAD_H
