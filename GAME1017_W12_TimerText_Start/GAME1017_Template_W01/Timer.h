#pragma once
#ifndef __TIMER__
#define __TIMER__
#include "GameObject.h"
#include <string>

class Timer : public GameObject
{
public:
	Timer();
	void Start();
	void Pause();
	void Resume();
	void Update();
	void Render() {}
	std::string GetTime();
	bool HasChanged();

private:
	bool   m_started, m_running, m_hasChanged;
	int    m_elapsed, m_elapsedTotal;
	time_t m_startTime, m_lastTime, m_currentTime;
};

#endif // !__TIMER__