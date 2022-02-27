#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include "Witch.h"
#include "Fireball.h"
#include "Bat.h"

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 5
using namespace std;

class Engine
{
private: //properties
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	//Textures
	SDL_Renderer* m_pRenderer;
	SDL_Texture* m_pwitch;
	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pFireball;
	SDL_Texture* m_pBatTexture;
	//Sound
	Mix_Chunk* m_pWitchFireSFX;
	Mix_Chunk* m_pBatFireSFX;
	Mix_Chunk* m_pDeathSFX;
	Mix_Music* m_pSpook4MUS;
	bool m_paused;
	//Objects
	Witch m_witch;
	vector<Fireball*> m_fireballs;
	vector<Bat*> m_bats;
	vector<Fireball*> m_batfire;
	//tools
	int m_spawnCtr = 0;
	int m_spawnMax = 150;
	int m_shootCtr = 0;
	int m_shootMax = 120;
	//Background
	SDL_Rect m_bg1 = { 0,0,1024,768 };
	SDL_Rect m_bg2 = { 1024,0,1024,768 };

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
	bool KeyDown(SDL_Scancode c);

};

#endif

// Reminder: you can ONLY have declarations (of variables and functions) in headers. What we have here is fine.
