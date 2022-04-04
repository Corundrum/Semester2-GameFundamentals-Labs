#pragma once
#ifndef _BOX_H_
#define _BOX_H_
#include <SDL.h>

class Obstacle 
{
private:
	//SDL_Rect m_src; //for A2
	SDL_Rect m_dst;
	SDL_Color m_color; //dont need for A2

public:
	Obstacle(); // If you have a dynamic array of Obstacle*
	Obstacle(const SDL_Rect r, const SDL_Color c);
	//void Update(); //may need for A2
	void Render();
	friend class Box; //alternately you can make a setter for m_dst
};

class Box // proxy for the obstacle
{
private:
	SDL_Point m_pos; // position on screen. Y is optional
	Obstacle* m_pSprite; // pointer to a dynamic sprite object
	int m_numSprites; // for bonus of A2

public:
	Box(const SDL_Point p, bool makeSprite = false, const SDL_Rect r = {0,0,0,0}, const SDL_Color c = {255,255,255,255});
	~Box();

	Box* Clone(); // making box a prototype
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }

};

#endif // !_BOX_H_