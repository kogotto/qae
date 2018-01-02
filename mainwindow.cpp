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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startSlot()
{
    receiveResult(QTime(0, 0, 0, 250), 250);
}

void MainWindow::stopSlot()
{
    receiveResult(QTime(0, 0, 0, 750), 750);
}

void MainWindow::receiveResult(QTime time, int result)
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
                 time.toString("zzz"));
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
