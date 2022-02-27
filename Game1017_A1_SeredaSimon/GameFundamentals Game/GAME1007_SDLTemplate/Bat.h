#pragma once
#ifndef _Bat_H_
#define _Bat_H_
#include <SDL.h>
#define MOVESPEED 3
class Bat
{
private:

public:
	SDL_Rect m_src, m_dst;
	Bat(int x = 0, int y = 0);
	void Update();
};

#endif