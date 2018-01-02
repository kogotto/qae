#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>

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
    QMessageBox::about(this, "kajds", "start button clicked");
}

void MainWindow::stopSlot()
{
    QMessageBox::about(this, "kdjf", "stop button clicked");
}
