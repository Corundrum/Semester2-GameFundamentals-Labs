#include <iostream>
#include <ctime>
#include "Engine.h"
#include "FontManager.h"
#include "TextureManager.h"
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
using namespace std;

Engine::Engine():m_bRunning(false){	cout << "Engine class constructed!" << endl; }
Engine::~Engine(){}

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				TEMA::Init();
				FOMA::Load("Img/ltype.TTF", "Label", 24);
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	srand((unsigned)time(NULL)); // Seed random number sequence.
	
	m_label = new Label("Label", 100, 100, "Time: 0:00");
	m_timer.Start();
	m_healthBar = new HealthBar(100);

	m_bRunning = true; // Everything is okay, start the engine.
	cout << "Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			m_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == 'w')
				m_label->SetColor({ 255,255,255,255 });
			else if (event.key.keysym.sym == 'g')
				m_label->SetColor({ 50,255,50,255 });
			else if (event.key.keysym.sym == 'b')
				m_label->SetColor({ 50,50,255,255 });
			
			else if (event.key.keysym.sym == 'd')
				m_healthBar->TakeDamage(5);
			
			if (event.key.keysym.sym == SDLK_p)
				m_timer.Pause();
			else if (event.key.keysym.sym == SDLK_r)
				m_timer.Resume();
			break;
		}
	}
}

// Keyboard utility function.
bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Engine::Update()
{
	m_timer.Update();
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	// Render stuff.
	if (m_timer.HasChanged())
	{
		string temp = "Time: " + m_timer.GetTime();
		m_label->SetText(temp.c_str());
	}

	m_label->Render();
	m_healthBar->Render();
	// Draw anew.
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	FOMA::Quit();
	TEMA::Quit();
	IMG_Quit();
	SDL_Quit();
}

int Engine::Run()
{
	if (m_bRunning) // What does this do and what can it prevent?
		return -1; 
	if (Init("GAME1017 Engine Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_bRunning) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_bRunning)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
