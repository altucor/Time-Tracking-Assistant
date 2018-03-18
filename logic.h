#ifndef LOGIC_H
#define LOGIC_H

#include <QThread>
#include <QString>
#include <QtimeEdit>
#include <QLabel>
#include <string>

class Logic : public QThread
{
    Q_OBJECT
public:
    Logic();
    ~Logic();
    void setMaxAfkTime(const QTimeEdit *obj);
    void setWorkDayLimit(const QTimeEdit *obj);
    void setFPS(unsigned long fps);
    void run();
    void resetTimer();
    unsigned long getLastInputTime();
    unsigned long getAfkTime();
    QString buildTime(unsigned long time);
    void updateScreen();
private:
    unsigned long m_fps = 10;
    unsigned long m_maxAfkTime = 0;
    unsigned long m_startTime = 0;
    unsigned long m_prevAfkTime = 0;
    unsigned long m_onlineTime = 0;
    unsigned long m_currentAfkTime = 0;
    unsigned long m_maxWorkDay = 0;
    unsigned long extractTimeFromQtimeEdit(const QTimeEdit *obj);
signals:
    void updMaxAfkTime(QString data);
    void updAfkTime(QString data);
    void updOnlineTime(QString data);
    void updPcUptime(QString data);
    void updRemaining(QString data);
    void updOvertime(QString data);
    void updAfkBar(int value);
    void updDayProgrBar(int value);
    void sendStatus(QString status);
public slots:
    //
};

#endif // LOGIC_H
