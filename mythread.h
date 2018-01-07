#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>
#include <QTime>


class Worker:
        public QObject
{
    Q_OBJECT

public slots:
    void doWork(int index);

signals:
    void sendResults(int index, QTime time, int result);
};


class Controller:
        public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

    void start();
    void pause();
    void stop();

signals:
    void resultsReady(int index, QTime time, int result);
    void finished();

    void doWork(int index);

private slots:
    void receiveResults(int index, QTime time, int result);

private:
    void doNextJob();

    QThread workerThread;
    int jobs;
    int currentJob;
    bool working;
};

#endif // MYTHREAD_H
