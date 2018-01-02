#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void startSlot();
    void stopSlot();

    void receiveResult(QTime time, int result);

private:
    void setTimeItem(int row, const QTime & time);
    void setResultItem(int row, int result);
    void setTableItem(int row, int column, const QString & text);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
