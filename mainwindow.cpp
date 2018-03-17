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
    this->setFixedSize(QSize(206, 258));
    m_timeLogicObj = new Logic();
    unsigned long fps = 60;
    m_timeLogicObj->setFPS(fps);
}

int MainWindow::init()
{
    QObject::connect(this, MainWindow::finished, m_timeLogicObj, Logic::terminate, Qt::DirectConnection);
    QObject::connect(m_timeLogicObj, &Logic::updateMaxAfkTime,  ui->label_1,    QLabel::setText);
    QObject::connect(m_timeLogicObj, &Logic::updateAfkTime,     ui->label_2,    QLabel::setText);
    QObject::connect(m_timeLogicObj, &Logic::updateOnlineTime,  ui->label_3,    QLabel::setText);
    QObject::connect(m_timeLogicObj, &Logic::updatePcUptime,    ui->label_4,    QLabel::setText);
    QObject::connect(m_timeLogicObj, &Logic::sendStatus,        this,           MainWindow::showStatus);
    QObject::connect(m_timeLogicObj, &Logic::updateAfkBar,      ui->afkBar,     QProgressBar::setValue);

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

void MainWindow::on_pushButton_clicked()
{
    m_timeLogicObj->setMaxAfkTime(ui->timeEdit);
    m_timeLogicObj->resetTimer();
    m_timeLogicObj->start();
}

void MainWindow::on_pushButton_2_clicked()
{
    m_timeLogicObj->terminate();
}
