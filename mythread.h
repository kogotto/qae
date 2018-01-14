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
    void doStage(int stage);

signals:
    void stageComplete(int stage);

private:
    void stage1();
    void stage2();
};

class JobController:
        public QObject
{
    Q_OBJECT
public:
    void start(int input);

signals:
    void doStage(int stage);

    void stageCompleteSignal(int stage);
    void completeSignal(int input);

public slots:
    void stageCompleteSlot(int stage);

private:
    void doNextStage();

    int input = 0;
    int currentStage = 0;
    bool paused = false;
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
