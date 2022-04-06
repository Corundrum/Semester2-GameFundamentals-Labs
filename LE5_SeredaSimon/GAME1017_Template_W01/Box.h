#pragma once
#ifndef _BOX_H_
#define _BOX_H_
#include <SDL.h>

class Obstacle
{
public:
	Obstacle(); // If you have a dynamic array of Obstacle*
	Obstacle(const SDL_Rect r, const SDL_Rect c);
	//void Update(); //may need for A2
	void Render();
	friend class Box; //alternately you can make a setter for m_dst

private:
	SDL_Rect m_dst, m_src;
	//SDL_Color m_color; //dont need for A2
};

class Box // proxy for the obstacle
{
public:
	Box(const SDL_Point p, bool makeSprite = false, const SDL_Rect r = { 0,0,0,0 }, const SDL_Rect c = { 0,0,0,0 });
	~Box();

	Box* Clone(); // making box a prototype
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }

private:
	SDL_Point m_pos; // position on screen. Y is optional
	Obstacle* m_pSprite; // pointer to a dynamic sprite object
	int m_numSprites; // for bonus of A2
};

#endif // !_BOX_H_