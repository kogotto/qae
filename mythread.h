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
    void doStage(int input, int stage);

signals:
    void stageComplete(int result, int stage);

private:
    void stage1(int input);
    void stage2(int input);
};


class WorkIterator {
public:
    WorkIterator() = default;
    explicit WorkIterator(size_t work, size_t stage = 1):
        work(work),
        stage(stage)
    {}

    WorkIterator & operator ++() {
        if (stage == stages) {
            ++work;
            stage = 1;
        } else {
            ++stage;
        }
        return *this;
    }

    bool operator == (const WorkIterator & rhs) const {
        return work == rhs.work &&
                stage == rhs.stage;
    }

    bool operator != (const WorkIterator & rhs) const {
        return !(operator==(rhs));
    }

    int work = 0;
    int stage = 0;
private:
    static const int stages = 2;
};

inline WorkIterator begin(const Work &) {
    return WorkIterator(0);
}

inline WorkIterator end(const Work & work) {
    return WorkIterator(work.size());
}


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

signals: // for the world
    void stageCompleteSignal(int currentIndex, int currentStage);
    void finished();

signals: // for worker
    void doStage(int input, int stage);

private slots:
    void stageCompleteSlot(int result, int stage);

private:
    void doNextJob();

    int currentInput() const;

    QThread workerThread;
    Work work;
    WorkIterator currentJob;
    WorkIterator endJob;


    State state;
};

#endif // MYTHREAD_H
