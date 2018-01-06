#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>


enum class Column {
    time = 0,
    result
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    worker = new Worker();
    worker->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished,
            worker, &QObject::deleteLater);

    connect(worker, &Worker::sendResults,
            this, &MainWindow::receiveResults);
    connect(this, &MainWindow::startWork,
            worker, &Worker::doWork);
    workerThread.start();
}

MainWindow::~MainWindow()
{
    delete ui;
    workerThread.quit();
    workerThread.wait();
}

void MainWindow::startSlot()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    emit startWork();
}

void MainWindow::stopSlot()
{
    emit stopWork();

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void MainWindow::receiveResults(QTime time, int result)
{
    const auto row = ui->table->rowCount();
    ui->table->insertRow(row);
    setTimeItem(row, time);
    setResultItem(row, result);
}

void MainWindow::setTimeItem(int row, const QTime &time)
{
    setTableItem(row,
                 static_cast<int>(Column::time),
                 time.toString("s.zzz"));
}

void MainWindow::setResultItem(int row, int result)
{
    setTableItem(row,
                 static_cast<int>(Column::result),
                 QString::number(result));
}

void MainWindow::setTableItem(int row, int column, const QString &text)
{
    auto item = new QTableWidgetItem(text);
    ui->table->setItem(row, column, item);
}
