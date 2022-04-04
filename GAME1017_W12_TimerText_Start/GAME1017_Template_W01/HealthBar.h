#pragma once
#ifndef __HEALTH_BAR__
#define __HEALTH_BAR__
#include "GameObject.h"

class HealthBar : public GameObject
{
public:
	HealthBar(int maxHealth);
	void Update();
	void Render();
	void TakeDamage(int amount);

private:
	SDL_FRect m_bar;
	int m_health;
};

#endif // !__HEALTH_BAR__