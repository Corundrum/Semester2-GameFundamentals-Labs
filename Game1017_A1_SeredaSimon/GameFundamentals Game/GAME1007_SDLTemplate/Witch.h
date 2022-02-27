#pragma once
#ifndef _WITCH_H_
#define _WITCH_H_
#include <SDL.h>
#define MOVESPEED 8
class Witch
{
private:

public:
	SDL_Rect m_src, m_dst;
	Witch(int x = 200, int y = 340);
};

#endif