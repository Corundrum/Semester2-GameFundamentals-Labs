#include "Timer.h"
#include <ctime>

Timer::Timer()
{
    m_started = m_running = m_hasChanged = m_elapsed = m_elapsedTotal = 0;

}

void Timer::Start()
{
    if (!m_started)
    {
        m_started = m_running = true;
        m_startTime = m_lastTime = m_currentTime = time(NULL);
    }
    else if (m_started && !m_running)
    {
        Resume();
    }
}

void Timer::Pause()
{
    if (m_running)
    {
        m_running = m_hasChanged = false;
        m_elapsedTotal += m_elapsed;
        m_elapsed = 0;
    }
}

void Timer::Resume()
{
    if (m_started && !m_running)
    {
        m_running = true;
        m_startTime = time(NULL);
    }
}

void Timer::Update()
{
    if (m_running)
    {
        m_lastTime = m_currentTime;
        m_currentTime = time(NULL);
        if (m_lastTime != m_currentTime)
        {
            m_hasChanged = true;
        }
        else
        {
            m_hasChanged = false;
        }
        m_elapsed = (int)m_currentTime - (int)m_startTime;
    }
}

std::string Timer::GetTime()
{
    if ((m_elapsedTotal + m_elapsed) % 60 < 10)
        return std::to_string((m_elapsedTotal + m_elapsed) / 60) + ":0" + std::to_string((m_elapsedTotal + m_elapsed) % 60);
    else
        return std::to_string((m_elapsedTotal + m_elapsed) / 60) + ':' + std::to_string((m_elapsedTotal + m_elapsed) % 60);

}

bool Timer::HasChanged()
{
    return m_hasChanged;
}
