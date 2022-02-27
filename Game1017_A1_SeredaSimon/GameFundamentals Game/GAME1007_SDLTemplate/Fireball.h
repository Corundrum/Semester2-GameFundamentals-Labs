#pragma once
#ifndef _FIREBALL_H_
#define _FIREBALL_H_
#include <SDL.h>
#define MOVESPEED 12
class Fireball
{
private:

public:
	SDL_Rect m_src, m_dst;
	Fireball(int x = 0, int y = 0);
	void Update();
	void UpdateReverse();
};

#endif