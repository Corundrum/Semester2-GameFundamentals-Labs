#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Primitives.h"
#include "Timer.h"
#include "HealthBar.h"
#include <iostream>
using namespace std;
// Do not include any macros, initialized properties or full function definitions in this header.

class Engine
{
private: // Private properties.
	bool m_bRunning; // Loop control flag.
	const Uint8* m_iKeystates; // Keyboard state container.
	Uint32 m_start, m_end, m_delta, m_fps; // Fixed timestep variables.
	SDL_Window* m_pWindow; // This represents the SDL window.
	SDL_Renderer* m_pRenderer; // This represents the buffer to draw to.

	// Example-specific properties.
	Label* m_label;
	Timer m_timer;
	HealthBar* m_healthBar;

private: // Private methods.
	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	void Wake();
	void Sleep();
	void HandleEvents();
	void Update();
	void Render();
	void Clean();
public: // Public methods.
	Engine();
	~Engine();
	int Run();
	static Engine& Instance(); // This static method creates the static instance that can be accessed 'globally'
	bool KeyDown(SDL_Scancode c);
	SDL_Renderer* GetRenderer();
};

#endif