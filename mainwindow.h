#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logic.h"

#include <QMainWindow>
#include <QSettings>
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
    QString m_version = "1.0";
    Logic *m_timeLogicObj;
    QString m_btnStartState = "";
    QString m_btnStopState = "";
    QString m_regPath = "HKEY_LOCAL_MACHINE\\Software\\AltucorSoftware\\TimeTrackingAssistant\\" + m_version;
    QString m_regMaxAfkTime = "MaxAfkTime";
    QString m_regWorkDay = "WorkDay";
    void checkRegistryValues();
    void updateRegistryValues();
signals:
    void finished();
private slots:
    void showStatus(QString message);
    void on_pushButton_Start_clicked();
    void on_pushButton_Stop_clicked();
};

#endif // MAINWINDOW_H
