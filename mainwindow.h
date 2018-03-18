#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logic.h"

#include <QMainWindow>
#include <QPalette>
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
    QString m_btnStartState = "";
    QString m_btnStopState = "";
signals:
    void finished();
private slots:
    void showStatus(QString message);
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
};

#endif // MAINWINDOW_H
