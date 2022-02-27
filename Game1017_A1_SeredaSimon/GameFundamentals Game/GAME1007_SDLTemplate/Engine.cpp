#include "Engine.h"
#include "StateManager.h"
#include "States.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	srand((unsigned)time(NULL));
	//If initialization is okay
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		//Create Window
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			//Create Renderer
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
		}
		else return false;
	}
	else return false;
	// Converts FPS into milliseconds, e.g. 16.67
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000);
	m_keystates = SDL_GetKeyboardState(nullptr);

	STMA::ChangeState(new MenuState());

	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}


void Engine::Wake()
{
	m_start = SDL_GetTicks();
	m_deltaTime = (m_start - m_startLast) / 1000.0;
	m_startLast = m_start;
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;
		}
	}
}

bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_keystates != nullptr)
	{
		if (m_keystates[c] == 1)
			return true;
	}
	return false;
}

void Engine::Update()
{
	STMA::Update();
}

void Engine::Render()
{
	//Clear Renderer
	SDL_RenderClear(m_pRenderer);

	STMA::Render();

	//Render Image
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);
}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance;
	return instance;
}

void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	
	STMA::Quit();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);

	Mix_CloseAudio();

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}