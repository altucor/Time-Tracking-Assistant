#include "logic.h"

#include <windows.h>
#include <sstream>

Logic::Logic()
{
    //
}

Logic::~Logic()
{
    //
}

unsigned long Logic::extractTimeFromQtimeEdit(const QTimeEdit *obj)
{
    int h = obj->time().hour();
    int m = obj->time().minute();
    int s = obj->time().second();

    unsigned long unsignedTime = s * 1000;
    unsignedTime += m * 1000 * 60;
    unsignedTime += h * 1000 * 60 * 60;

    return unsignedTime;
}

void Logic::setMaxAfkTime(const QTimeEdit *obj)
{
    m_maxAfkTime = extractTimeFromQtimeEdit(obj);
}

void Logic::setWorkDayLimit(const QTimeEdit *obj)
{
    m_maxWorkDay = extractTimeFromQtimeEdit(obj);
}

void Logic::setFPS(unsigned long fps)
{
    m_fps = 1000 / fps;
}

void Logic::run()
{
    while(1){
        updateScreen();
        Sleep(m_fps);
        m_prevAfkTime = getLastInputTime();
    }
}

void Logic::resetTimer()
{
    m_startTime = GetTickCount();
    m_onlineTime = 0;
}

unsigned long Logic::getLastInputTime()
{
    LASTINPUTINFO pInput;
    pInput.cbSize = sizeof(LASTINPUTINFO);

    if(!GetLastInputInfo(&pInput)){
        // error
    }
    return pInput.dwTime;
}

unsigned long Logic::getAfkTime()
{
    return GetTickCount() - getLastInputTime();
}

void Logic::setPauseTime()
{
    m_pauseTime = GetTickCount();
}

void Logic::setContinueTime()
{
    m_continueTime = GetTickCount();
    m_startTime += m_continueTime - m_pauseTime;
}

QString Logic::buildTime(unsigned long time)
{
    unsigned long ms = time % 1000;
    time /= 1000;
    unsigned long s = time % 60;
    time /= 60;
    unsigned long m = time % 60;
    time /= 60;
    unsigned long h = time;

    std::stringstream timeStream;
    if(h < 10){timeStream << "0" << h;}else{timeStream << h;} timeStream << ":";
    if(m < 10){timeStream << "0" << m;}else{timeStream << m;} timeStream << ":";
    if(s < 10){timeStream << "0" << s;}else{timeStream << s;} timeStream << ":";
    if(ms < 10){timeStream << "00" << ms;}else{timeStream << ms;};
    return QString::fromStdString(timeStream.str());
}

void Logic::updateScreen()
{
    if(getAfkTime() == 0 && m_onlineTime > m_maxAfkTime){ //reset
        m_startTime += m_currentAfkTime;
    }

    if(getAfkTime() > m_maxAfkTime && m_onlineTime > m_maxAfkTime){ // adding afk time
        m_currentAfkTime = getAfkTime();
    } else {
        m_currentAfkTime = 0;
    }

    m_onlineTime = GetTickCount() - m_startTime;
    //-----------------------------------------------------------------
    emit updAfkTime( buildTime( getAfkTime() ) );
    emit updOnlineTime(  buildTime( m_onlineTime ) );
    emit updPcUptime( buildTime( GetTickCount() ) );
    //-----------------------------------------------------------------
    int afkBarValue = getAfkTime() * 100 / m_maxAfkTime;
    emit updAfkBar(afkBarValue);
    //-----------------------------------------------------------------
    int dayProgressTime = m_onlineTime * 100 / m_maxWorkDay;
    emit updDayProgrBar(dayProgressTime);
    //-----------------------------------------------------------------
    unsigned long diffTime = m_maxWorkDay - m_onlineTime;
    if(diffTime > m_maxWorkDay){
        emit updRemaining( m_fontGreen.arg( buildTime( 0 ) ) );
        emit updOvertime( m_fontRed.arg( buildTime( diffTime * -1 ) ) );
    } else {
        emit updRemaining( m_fontBlack.arg( buildTime( diffTime ) ) );
        emit updOvertime( m_fontBlack.arg( buildTime( 0 ) ) );
    }
    //-----------------------------------------------------------------
    if(GetTickCount() - m_prevAfkTime > m_maxAfkTime){
        emit sendStatus("Status: Offline");
    } else {
        emit sendStatus("Status: Online");
    }
}
