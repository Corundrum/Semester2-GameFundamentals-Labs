#include "Fireball.h"

Fireball::Fireball(int x, int y) :m_src({4,6,10,5})
{
	m_dst = { x,y,m_src.w*4,m_src.h*4 };
}

void Fireball::Update()
{
	m_dst.x += MOVESPEED;
}

void Fireball::UpdateReverse()
{
	m_dst.x -= MOVESPEED;
}
