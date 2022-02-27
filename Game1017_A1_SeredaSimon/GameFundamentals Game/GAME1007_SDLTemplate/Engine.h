#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <iostream>
#include <vector>
#include <ctime>

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 5
using namespace std;

class Engine
{
private: //properties
	bool m_running = false;
	Uint32 m_start, m_startLast, m_end, m_delta, m_fps;
	double m_deltaTime;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;

	SDL_Renderer* m_pRenderer;


private: //method prototypes
	Engine() { std::cout << "Engine Created..." << std::endl; }
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Wake();
	void HandleEvents();
	void Update();
	void Render();
	void Sleep();
	void Clean();


public: // public method prototyples
	int Run();

	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	double GetDeltaTime() const { return m_deltaTime; }
	bool KeyDown(SDL_Scancode c);

};

#endif

// Reminder: you can ONLY have declarations (of variables and functions) in headers. What we have here is fine.
