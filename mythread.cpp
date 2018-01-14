#include "mythread.h"

#include <cassert>


void Worker::doStage(int stage) {
    switch (stage) {
        case 1: return stage1();
        case 2: return stage2();
        default:
            assert(false && "unknown stage");
    }
}

void Worker::stage1()
{
    const auto time = qrand() % 500 + 1000;
    QThread::msleep(time);
    emit stageComplete(1);
}

void Worker::stage2()
{
    const auto time = qrand() % 500 + 1000;
    QThread::msleep(time);
    emit stageComplete(2);
}


void JobController::start(int input) {
    this->input = input;
    currentStage = 1;
    paused = false;
    doNextStage();
}

void JobController::stageCompleteSlot(int stage) {
    assert(stage == currentStage);
    emit stageComplete(stage);
    ++currentStage;
    if (currentStage > 2) {
        emit completeSignal(input);
        return;
    }

    doNextStage();
}

void JobController::doNextStage() {
    if (!paused) {
        emit doStage(currentStage);
    }
}


Controller::Controller():
    jobs(0),
    currentJob(0),
    working(false)
{
    auto worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished,
            worker, &QObject::deleteLater);

    connect(&jc, &JobController::doStage,
            worker, &Worker::doStage);
    connect(worker, &Worker::stageComplete,
            &jc, &JobController::stageCompleteSlot);

    workerThread.start();
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::start(const Work &work)
{
    this->work = &work;
    currentJob = 0;
    working = true;

    doNextJob();
}

void Controller::stop()
{
    working = false;
}

void Controller::receiveResults(int index, QTime time, int result)
{
    assert(index == currentJob);
    emit resultsReady(index, time, result);

    ++currentJob;
    if (currentJob == jobs) {
        emit finished();
        return;
    }

    doNextJob();
}

void Controller::doNextJob()
{
    if (working) {
        emit doWork(currentJob, currentInput());
    }
}

int Controller::currentInput()
{
    return (*work->table)[currentJob].input;
}
