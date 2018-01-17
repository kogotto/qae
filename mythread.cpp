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

void JobController::stop()
{
  paused = true;
}

void JobController::stageCompleteSlot(int stage) {
    assert(stage == currentStage);
    emit stageCompleteSignal(stage);
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


Controller:: Controller():
    currentJob(0),
    state(State::idle)
{
    auto worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished,
            worker, &QObject::deleteLater);

    connect(&jc, &JobController::doStage,
            worker, &Worker::doStage);
    connect(worker, &Worker::stageComplete,
            &jc, &JobController::stageCompleteSlot);

    connect(&jc, &JobController::stageCompleteSignal,
            this, &Controller::stageCompleteSlot);
    connect(&jc, &JobController::completeSignal,
            this, &Controller::workComplete);

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
    jc.stop();
}

Controller::State Controller::getState() const
{
  return state;
}

void Controller::stageCompleteSlot(int input, int stage)
{
    assert(input == currentInput());
    emit stageCompleteSignal(currentJob, stage);
}

void Controller::workComplete(int input)
{
    emit workCompleteSignal(currentJob);
    ++currentJob;

    if (currentJob == work.size()) {
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
