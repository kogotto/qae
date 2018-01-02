#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

#include "mythread.h"

enum class Column {
    time = 0,
    result
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSlot()
{
    ui->startButton->setEnabled(false);
    ui->stopButton->setEnabled(true);

    auto thread = new MyThread();
    connect(thread, &MyThread::sendResults,
            this, &MainWindow::receiveResults);
    connect(thread, &MyThread::finished,
            thread, &QObject::deleteLater);
    thread->start();
}

void MainWindow::stopSlot()
{
    receiveResults(QTime(0, 0, 0, 750), 750);
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
