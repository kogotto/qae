#include "mythread.h"

#include <cassert>


void Worker::doWork(int index)
{
    const auto time = qrand() % 2000 + 1000;
    const auto result = qrand() % 10;
    QThread::msleep(time);
    emit sendResults(index, QTime(0, 0).addMSecs(time), result);
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

    connect(worker, &Worker::sendResults,
            this, Controller::receiveResults);
    connect(this, &Controller::doWork,
            worker, &Worker::doWork);

    connect(this, &Controller::doWork,
            &jc, &JobController::doWork);

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

JobController::JobController(QThread &thread)
{

}

void JobController::doWork(int index, int input)
{

}
