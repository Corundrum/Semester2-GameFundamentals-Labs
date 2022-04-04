#include "HealthBar.h"
#include "Engine.h"

HealthBar::HealthBar(int maxHealth) : m_health(maxHealth)
{
	m_dst = {512,100,100,26};
	m_bar = {514,102,96,22};
}

void HealthBar::Update()
{
	
	//cosmetic update function
}

void HealthBar::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255,255,255,255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), GetDst());

	double perc = (m_health / 100.0);
	m_bar.w = 96 * perc;
	std::cout << perc << std::endl;
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255 * (1 - perc), 255 * perc, 0, 255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_bar);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);

}

void HealthBar::TakeDamage(int amount)
{
	m_health -= amount;
	if (m_health < 0) m_health = 0;
	if (m_health > 100) m_health = 100;
}
