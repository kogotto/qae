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

    model = new Model();
    ui->table->setModel(model);

    connect(&controller, &Controller::stageCompleteSignal,
            this, &MainWindow::stageCompleteSlot);
    connect(&controller, &Controller::finished,
            this, &MainWindow::finishedSlot);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete model;
}

void MainWindow::startSlot()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    if (controller.getState() == Controller::State::idle)
      controller.start(model->getWork());
    else if (controller.getState() == Controller::State::interapted)
      controller.cont();
}

void MainWindow::stopSlot()
{
    controller.stop();

    ui->startButton->setEnabled(true);
    ui->stopButton->setEnabled(false);
}

void MainWindow::resetSlot()
{

}

void MainWindow::addRowSlot()
{
  model->insertRow(model->rowCount());
}

void MainWindow::finishedSlot()
{
  ui->startButton->setEnabled(true);
  ui->stopButton->setEnabled(false);
}

void MainWindow::stageCompleteSlot(int job, int stage)
{
  model->setData(model->index(job, stage), true);
}

void MainWindow::workCompleteSlot(int job)
{
  model->setData(model->index(job, 3), true);
}

//void MainWindow::receiveResults(int index, QTime time, int result)
//{
//    if (index >= ui->table->rowCount()) {
//        ui->table->setRowCount(index + 1);
//    }

//    setTimeItem(index, time);
//    setResultItem(index, result);
//}

//void MainWindow::setTimeItem(int row, const QTime &time)
//{
//    setTableItem(row,
//                 static_cast<int>(Column::time),
//                 time.toString("s.zzz"));
//}

//void MainWindow::setResultItem(int row, int result)
//{
//    setTableItem(row,
//                 static_cast<int>(Column::result),
//                 QString::number(result));
//}

//void MainWindow::setTableItem(int row, int column, const QString &text)
//{
//    auto item = new QTableWidgetItem(text);
//    ui->table->setItem(row, column, item);
//}
