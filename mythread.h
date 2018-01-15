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
    void stop();

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
    enum class State {
        idle
      , working
      , interapted
    };


    Controller();
    ~Controller();

    void start(const Work & work);
    void cont();
    void pause();
    void stop();

    State getState() const;

signals:
    void stageCompleteSignal(int currentIndex, int currentStage);
    void workCompleteSignal(int currentIndex);
    void finished();

private slots:
    void stageCompleteSlot(int stage);
    void workComplete(int input);

private:
    void doNextJob();

    int currentInput();

    QThread workerThread;
    Work work;
    JobController jc;
    int currentJob;

    State state;
};

#endif // MYTHREAD_H
