#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logic.h"

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
    int init();
    void work();
private:
    Ui::MainWindow *ui;
    Logic *m_timeLogicObj;
signals:
    void finished();

private slots:
    void showStatus(QString message);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // MAINWINDOW_H