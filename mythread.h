#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>
#include <QTime>

#include "work.h"


class Worker:
        public QObject
{
    Q_OBJECT

public slots:
    void doWork(int index);

signals:
    void sendResults(int index, QTime time, int result);
};

class JobController:
        public QObject
{
    Q_OBJECT
public:
    JobController(QThread & thread);
public slots:
    void doWork(int index, int input);

};

class Controller:
        public QObject
{
    Q_OBJECT
public:
    Controller();
    ~Controller();

    void start(const Work & work);
    void pause();
    void stop();

signals:
    void resultsReady(int index, QTime time, int result);
    void finished();

    void doWork(int index, int input);

private slots:
    void receiveResults(int index, QTime time, int result);

private:
    void doNextJob();

    int currentInput();

    QThread workerThread;
    Work * work;
    JobController jc;
    int currentJob;
    bool working;
};

#endif // MYTHREAD_H
