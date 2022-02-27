#include "Bat.h"

Bat::Bat(int x, int y) :m_src({ 15,85,17,21 })
{
	m_dst = { x,y,m_src.w * 2,m_src.h * 2 };
}

void Bat::Update()
{
	m_dst.x -= MOVESPEED;
}
