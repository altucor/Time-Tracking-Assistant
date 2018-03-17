#ifndef LOGIC_H
#define LOGIC_H

#include <QThread>
#include <QString>
#include <QtimeEdit>
#include <string>

class Logic : public QThread
{
    Q_OBJECT
public:
    Logic();
    ~Logic();
    void setMaxAfkTime(const QTimeEdit *obj);
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
signals:
    void updateMaxAfkTime(QString data);
    void updateAfkTime(QString data);
    void updateOnlineTime(QString data);
    void updatePcUptime(QString data);
    void sendStatus(QString status);
    void updateAfkBar(int value);
public slots:
    //
};

#endif // LOGIC_H
