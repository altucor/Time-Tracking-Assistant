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
    unsigned long extractTimeFromQtimeEdit(const QTimeEdit *obj);
    void setFPS(unsigned long fps);
    void run();
    void resetTimer();
    unsigned long getLastInputTime();
    unsigned long getAfkTime();
    QString buildTime(unsigned long time);
    void setPauseTime();
    void setContinueTime();
    void updateScreen();
private:
    unsigned long m_fps = 10;
    unsigned long m_maxAfkTime = 0;
    unsigned long m_startTime = 0;
    unsigned long m_prevAfkTime = 0;
    unsigned long m_onlineTime = 0;
    unsigned long m_currentAfkTime = 0;
    unsigned long m_maxWorkDay = 0;
    unsigned long m_pauseTime = 0;
    unsigned long m_continueTime = 0;
    /// Colors
    QString m_fontRed   = tr("<font color='Red'>%1</font>");
    QString m_fontGreen = tr("<font color='Green'>%1</font>");
    QString m_fontBlue  = tr("<font color='Blue'>%1</font>");
    QString m_fontBlack = tr("<font color='Black'>%1</font>");
signals:
    void updAfkTime(const QString &data);
    void updOnlineTime(const QString &data);
    void updPcUptime(const QString &data);
    void updRemaining(const QString &data);
    void updOvertime(const QString &data);
    void updAfkBar(int value);
    void updDayProgrBar(int value);
    void sendStatus(const QString &status);
public slots:
    //
};

#endif // LOGIC_H
