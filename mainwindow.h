#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>
#include <QThread>
#include "mythread.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void startWork();
    void stopWork();

private slots:
    void startSlot();
    void stopSlot();

    void receiveResults(int index, QTime time, int result);

private:
    void setTimeItem(int row, const QTime & time);
    void setResultItem(int row, int result);
    void setTableItem(int row, int column, const QString & text);

    Ui::MainWindow *ui;
    Controller controller;
};

#endif // MAINWINDOW_H
