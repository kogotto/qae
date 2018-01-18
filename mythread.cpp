#include "mythread.h"

#include <cassert>


void Worker::doStage(int input, int stage) {
    switch (stage) {
        case 1: return stage1(input);
        case 2: return stage2(input);
        default:
            assert(false && "unknown stage");
    }
}

void Worker::stage1(int input)
{
    const auto time = qrand() % 500 + 1000;
    QThread::msleep(time);
    const int result = input * 2;
    emit stageComplete(result, 1);
}

void Worker::stage2(int input)
{
    const auto time = qrand() % 500 + 1000;
    QThread::msleep(time);
    const int result = input + 10;
    emit stageComplete(result, 2);
}


Controller:: Controller():
    currentJob(0),
    state(State::idle)
{
    auto worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished,
            worker, &QObject::deleteLater);

    connect(this, &Controller::doStage,
            worker, &Worker::doStage);
    connect(worker, &Worker::stageComplete,
            this, &Controller::stageCompleteSlot);

    workerThread.start();
}

Controller::~Controller()
{
    workerThread.quit();
    workerThread.wait();
}

void Controller::start(const Work &work)
{
    this->work = work;
    currentJob = begin(work);
    endJob = end(work);
    state = State::working;

    doNextJob();
}

void Controller::cont()
{
  assert(state == State::interapted);
  state = State::working;

  doNextJob();
}

void Controller::stop()
{
    state = State::interapted;
}

Controller::State Controller::getState() const
{
  return state;
}

void Controller::stageCompleteSlot(int result, int stage)
{
    assert(currentJob.stage == stage);
    emit stageCompleteSignal(currentJob.work, stage);

    ++currentJob;
    if (currentJob == endJob) {
        state = State::idle;
        emit finished();
        return;
    }

    doNextJob();
}

void Controller::doNextJob()
{
    if (state == State::working) {
        emit doStage(currentInput(), currentJob.stage);
    }
}

int Controller::currentInput() const
{
    return work[currentJob.work];
}
