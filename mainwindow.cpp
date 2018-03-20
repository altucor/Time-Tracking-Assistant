#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QObject>
#include <QLabel>
#include <QCheckBox>
#include <QSlider>
#include <ctime>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(QSize(200, 400));
    m_timeLogicObj = new Logic();
    unsigned long fps = 60;
    m_timeLogicObj->setFPS(fps);
}

int MainWindow::init()
{
    QObject::connect(this,              MainWindow::finished,       m_timeLogicObj,                 Logic::terminate, Qt::DirectConnection);
    QObject::connect(m_timeLogicObj,    &Logic::updAfkTime,         ui->dynamic_Label_AfkTime,      QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updOnlineTime,      ui->dynamic_Label_OnlineTime,   QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updPcUptime,        ui->dynamic_Label_PCUptime,     QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updRemaining,       ui->dynamic_Label_Remaining,    QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updOvertime,        ui->dynamic_Label_Overtime,     QLabel::setText);
    QObject::connect(m_timeLogicObj,    &Logic::updAfkBar,          ui->barAfk,                     QProgressBar::setValue);
    QObject::connect(m_timeLogicObj,    &Logic::updDayProgrBar,     ui->barDayProgress,             QProgressBar::setValue);
    QObject::connect(m_timeLogicObj,    &Logic::sendStatus,         this,                           MainWindow::showStatus);
    QObject::connect(ui->checkBox_onTop,&QCheckBox::stateChanged,   this,                           MainWindow::setOnTopFlag);

    QObject::connect(ui->horizontalSlider_opacity, QSlider::valueChanged, this, MainWindow::changeOpacity);
    checkRegistryValues();
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
    updateRegistryValues();
    emit finished();
    delete ui;
}

void MainWindow::checkRegistryValues()
{
    QSettings winRegistry(m_regPath);
    QTime emptyObject;
    emptyObject.setHMS(0, 0, 0, 0);
    QTime MaxAfkTime = winRegistry.value(m_regMaxAfkTime).toTime();
    QTime WorkDay = winRegistry.value(m_regWorkDay).toTime();
    int OnTopFlag = winRegistry.value(m_regOnTopFlag).toInt();
    int WindowOpacity = winRegistry.value(m_regOpacity).toInt();

    if(MaxAfkTime != ui->timeEdit_AfkTime->time() && MaxAfkTime != emptyObject){
        ui->timeEdit_AfkTime->setTime(MaxAfkTime);
    }

    if(WorkDay != ui->timeEdit_WorkDayTime->time() && WorkDay != emptyObject){
        ui->timeEdit_WorkDayTime->setTime(WorkDay);
    }

    ui->checkBox_onTop->setChecked(OnTopFlag);

    if(WindowOpacity < 5){ // if its too low than fix that
        WindowOpacity = 100;
        winRegistry.setValue(m_regOpacity,QVariant(100));
    }
    ui->horizontalSlider_opacity->setValue(WindowOpacity);
}

void MainWindow::updateRegistryValues()
{
    QSettings winRegistry(m_regPath);
    QVariant qvarMaxAfkTime = ui->timeEdit_AfkTime->time();
    QVariant qvarWorkDay = ui->timeEdit_WorkDayTime->time();
    QVariant qvarOnTopFlag = ui->checkBox_onTop->checkState();
    QVariant qvarOpacityWindow = ui->horizontalSlider_opacity->value();

    winRegistry.setValue(m_regMaxAfkTime, qvarMaxAfkTime);
    winRegistry.setValue(m_regWorkDay, qvarWorkDay);
    winRegistry.setValue(m_regOnTopFlag, qvarOnTopFlag);
    winRegistry.setValue(m_regOpacity, qvarOpacityWindow);
}

void MainWindow::setOnTopFlag(int state)
{
    if(state == 2){
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    }else if(state == 0){
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    }
    show();
}

void MainWindow::changeOpacity(int value)
{
    double realVal = ((double)value / (double)100);
    this->setWindowOpacity(realVal);
}

void MainWindow::showStatus(QString message)
{
    ui->statusBar->showMessage(message, 1000);
}

void MainWindow::resetAllValues()
{
    QString nullTimeStr = "00:00:00:000";
    ui->dynamic_Label_AfkTime->setText(nullTimeStr);
    ui->dynamic_Label_OnlineTime->setText(nullTimeStr);
    ui->dynamic_Label_Remaining->setText(nullTimeStr);
    ui->dynamic_Label_PCUptime->setText(nullTimeStr);
    ui->dynamic_Label_Overtime->setText(nullTimeStr);
    ui->barAfk->setValue(0);
    ui->barDayProgress->setValue(0);
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
        ui->pushButton_Stop->setEnabled(true);
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
        ui->pushButton_Stop->setEnabled(false);
        m_timeLogicObj->terminate();
        resetAllValues();
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
