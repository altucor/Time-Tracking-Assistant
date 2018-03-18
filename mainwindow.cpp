#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QObject>
#include <QLabel>
#include <ctime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(200, 380));
    m_timeLogicObj = new Logic();
    unsigned long fps = 60;
    m_timeLogicObj->setFPS(fps);
}

int MainWindow::init()
{
    QObject::connect(this,              MainWindow::finished,   m_timeLogicObj,                 Logic::terminate, Qt::DirectConnection);
    QObject::connect(m_timeLogicObj,    &Logic::updAfkTime,     ui->dynamic_Label_AfkTime,      QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updOnlineTime,  ui->dynamic_Label_OnlineTime,   QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updPcUptime,    ui->dynamic_Label_PCUptime,     QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updRemaining,   ui->dynamic_Label_Remaining,    QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updOvertime,    ui->dynamic_Label_Overtime,     QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updAfkBar,      ui->barAfk,                     QProgressBar::setValue);
    QObject::connect(m_timeLogicObj,    &Logic::updDayProgrBar, ui->barDayProgress,             QProgressBar::setValue);
    QObject::connect(m_timeLogicObj,    &Logic::sendStatus,     this,                           MainWindow::showStatus);

    return 0;
}

void MainWindow::work()
{
    //
}

MainWindow::~MainWindow()
{
    m_timeLogicObj->terminate();
    delete m_timeLogicObj;
    emit finished();
    delete ui;
}

void MainWindow::showStatus(QString message)
{
    ui->statusBar->showMessage(message, 1000);
}

void MainWindow::on_pushButton_Start_clicked()
{
    QString btnText = ui->pushButton_Start->text();
    if(btnText == "Start" || btnText == "Restart"){
        ui->timeEdit_AfkTime->setEnabled(false);
        ui->timeEdit_WorkDayTime->setEnabled(false);

        m_timeLogicObj->setMaxAfkTime(ui->timeEdit_AfkTime);
        m_timeLogicObj->setWorkDayLimit(ui->timeEdit_WorkDayTime);
        m_timeLogicObj->resetTimer();
        m_timeLogicObj->start();

        ui->pushButton_Start->setText("Restart");
        ui->pushButton_Stop->setText("Pause");
    } else if(btnText == "Resume"){
        m_timeLogicObj->setContinueTime();
        m_timeLogicObj->start();
        ui->pushButton_Start->setText("Restart");
        ui->pushButton_Stop->setText("Pause");
    }
}

void MainWindow::on_pushButton_Stop_clicked()
{
    QString btnText = ui->pushButton_Stop->text();
    if(btnText == "Stop"){
        m_timeLogicObj->terminate();

        ui->timeEdit_AfkTime->setEnabled(true);
        ui->timeEdit_WorkDayTime->setEnabled(true);
        ui->pushButton_Start->setText("Start");
    } else if(btnText == "Pause") {
        m_timeLogicObj->setPauseTime();
        m_timeLogicObj->terminate();
        ui->pushButton_Start->setText("Resume");
        ui->pushButton_Stop->setText("Stop");
    }
}
