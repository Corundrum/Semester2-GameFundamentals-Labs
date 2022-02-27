#include "Engine.h"

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
			if (m_pRenderer != nullptr)
			{	//Create Texture
				if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
				{
					m_pwitch = IMG_LoadTexture(m_pRenderer, "witch.png");
					m_pBGTexture = IMG_LoadTexture(m_pRenderer, "sky_clouds.jpg");
					m_pFireball = IMG_LoadTexture(m_pRenderer, "Fireball.png");
					m_pBatTexture = IMG_LoadTexture(m_pRenderer, "bat.png");
				}
				else return false;
				//Create sounds
				if (Mix_Init(MIX_INIT_MP3) != 0)
				{
					//configure mixer
					Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 4096);
					Mix_AllocateChannels(16);
					//load sound
					m_pWitchFireSFX = Mix_LoadWAV("audio/witchfire.wav");
					m_pBatFireSFX = Mix_LoadWAV("audio/batfire.wav");
					m_pDeathSFX = Mix_LoadWAV("audio/death.wav");
					m_pSpook4MUS = Mix_LoadMUS("audio/spook4.mp3");
				}
				else return false;
			}
			else return false;
		}
		else return false;
	}
	else return false;
	// Converts FPS into milliseconds, e.g. 16.67
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000);
	m_keystates = SDL_GetKeyboardState(nullptr);

	Mix_VolumeMusic(16);
	Mix_Volume(-1, 32);
	Mix_PlayMusic(m_pSpook4MUS, -1);

	cout << "Initialization successful!" << endl;
	m_running = true;
	return true;
}


void Engine::Wake()
{
	m_start = SDL_GetTicks();
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
		case SDL_KEYUP:
			if (event.key.keysym.sym == ' ')
			{
				Mix_PlayChannel(-1, m_pWitchFireSFX, 0);
				m_fireballs.push_back(new Fireball((m_witch.m_dst.x + 64), (m_witch.m_dst.y + 64)));
				m_fireballs.shrink_to_fit();
			}
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
	if (KeyDown(SDL_SCANCODE_W) && m_witch.m_dst.y > 0)
	{
		m_witch.m_dst.y -= SPEED;
	}
	if (KeyDown(SDL_SCANCODE_A) && m_witch.m_dst.x > 0)
	{
		m_witch.m_dst.x -= SPEED;
	}
	if (KeyDown(SDL_SCANCODE_S) && m_witch.m_dst.y < (HEIGHT - m_witch.m_dst.h))
	{
		m_witch.m_dst.y += SPEED;
	}
	if (KeyDown(SDL_SCANCODE_D) && m_witch.m_dst.x < (WIDTH/2 - m_witch.m_dst.w))
	{
		m_witch.m_dst.x += SPEED;
	}

	for (unsigned i = 0; i < m_fireballs.size(); i++)
	{
		m_fireballs[i]->Update();
	}

	for (unsigned i = 0; i < m_fireballs.size(); i++)
	{
		if (m_fireballs[i]->m_dst.x >= 1024)
		{
			delete m_fireballs[i];
			m_fireballs[i] = nullptr;
			m_fireballs.erase(m_fireballs.begin() + i);
			m_fireballs.shrink_to_fit();

			break;
		}
	}

	//Scroll Backgrounds
	m_bg1.x -= SPEED;
	m_bg2.x -= SPEED;

	if (m_bg1.x <= -m_bg1.w)
	{
		m_bg1.x = 0;
		m_bg2.x = 1024;
	}


	//Spawn Enemies
	if (m_spawnCtr++ == m_spawnMax)
	{
		m_spawnCtr = 0;
		m_bats.push_back(new Bat(1024, (rand() % 680)));
		m_bats.shrink_to_fit();
	}
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		m_bats[i]->Update();
	}
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		if (m_bats[i]->m_dst.x <= -m_bats[i]->m_dst.w)
		{
			delete m_bats[i];
			m_bats[i] = nullptr;
			m_bats.erase(m_bats.begin() + i);
			m_bats.shrink_to_fit();

			break;
		}
	}

	//Bat fireball
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		if (m_shootCtr++ == m_shootMax)
		{
			m_shootCtr = 0;
			Mix_PlayChannel(-1, m_pBatFireSFX, 0);
			m_batfire.push_back(new Fireball(m_bats[i]->m_dst.x, (m_bats[i]->m_dst.y + 12)));
			m_batfire.shrink_to_fit();
		}
	}
	for (unsigned i = 0; i < m_batfire.size(); i++)
	{
		m_batfire[i]->UpdateReverse();
	}
	for (unsigned i = 0; i < m_batfire.size(); i++)
	{
		if (m_batfire[i]->m_dst.x <= -m_batfire[i]->m_dst.w)
		{
			delete m_batfire[i];
			m_batfire[i] = nullptr;
			m_batfire.erase(m_batfire.begin() + i);
			m_batfire.shrink_to_fit();

			break;
		}
	}

	//Collision for fireball to bats
	for (unsigned i = 0; i < m_fireballs.size(); i++)
	{
		for (unsigned j = 0; j < m_bats.size(); j++)
		{
			if (SDL_HasIntersection(&m_fireballs[i]->m_dst, &m_bats[j]->m_dst))
			{
				Mix_PlayChannel(-1, m_pDeathSFX, 0);
				delete m_fireballs[i];
				delete m_bats[j];
				m_fireballs[i] = nullptr;
				m_bats[j] = nullptr;
				m_fireballs.erase(m_fireballs.begin() + i);
				m_bats.erase(m_bats.begin() + j);
				m_fireballs.shrink_to_fit();
				m_bats.shrink_to_fit();
				break;
			}
		}
	}
	//Collision for batfire to witch
	for (unsigned i = 0; i < m_batfire.size(); i++)
	{
		if (SDL_HasIntersection(&m_batfire[i]->m_dst, &m_witch.m_dst))
		{
			Mix_PlayChannel(-1, m_pDeathSFX, 0);
			delete m_batfire[i];
			m_batfire[i] = nullptr;
			m_batfire.erase(m_batfire.begin() + i);
			m_batfire.shrink_to_fit();
			//delete m_pwitch;
			SDL_DestroyTexture(m_pwitch);
			break;
		}
	}
	//Collision for bats to witch
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		if (SDL_HasIntersection(&m_bats[i]->m_dst, &m_witch.m_dst))
		{
			Mix_PlayChannel(-1, m_pDeathSFX, 0);
			delete m_bats[i];
			m_bats[i] = nullptr;
			m_bats.erase(m_bats.begin() + i);
			m_bats.shrink_to_fit();
			//delete m_pwitch;
			SDL_DestroyTexture(m_pwitch);
			break;
		}
	}


	//cout << "cap: " << m_batfire.capacity() << " sz: " << m_batfire.size() << endl;
	//cout << "cap: " << m_bats.capacity() << " sz: " << m_bats.size() << endl;
	//cout << "cap: " << m_fireballs.capacity() << " sz: " << m_fireballs.size() << endl;
}

void Engine::Render()
{
	//Clear Renderer
	SDL_RenderClear(m_pRenderer);

	//Backgrounds
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopy(m_pRenderer, m_pBGTexture, NULL, &m_bg2);
	//Witch
	SDL_RenderCopy(m_pRenderer, m_pwitch, &m_witch.m_src, &m_witch.m_dst);
	//Fireballs
	for (unsigned i = 0; i < m_fireballs.size(); i++)
	{
		SDL_SetTextureColorMod(m_pFireball, 255, 255, 255);
		SDL_RenderCopy(m_pRenderer, m_pFireball, &(m_fireballs[i]->m_src), &(m_fireballs[i]->m_dst));
	}
	//bats
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		SDL_RenderCopy(m_pRenderer, m_pBatTexture, &(m_bats[i]->m_src), &(m_bats[i]->m_dst));
	}
	//Enemy Fireballs
	for (unsigned i = 0; i < m_batfire.size(); i++)
	{
		SDL_SetTextureColorMod(m_pFireball, 0, 255, 255);
		SDL_RenderCopyEx(m_pRenderer, m_pFireball, &(m_batfire[i]->m_src), &(m_batfire[i]->m_dst), NULL, NULL, SDL_FLIP_HORIZONTAL);
	}
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
	//fireballs
	for (unsigned i = 0; i < m_fireballs.size(); i++)
	{
		delete m_fireballs[i];
		m_fireballs[i] = nullptr;
	}
	m_fireballs.clear();
	m_fireballs.shrink_to_fit();
	//enemies
	for (unsigned i = 0; i < m_bats.size(); i++)
	{
		delete m_bats[i];
		m_bats[i] = nullptr;
	}
	m_bats.clear();
	m_bats.shrink_to_fit();

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyTexture(m_pwitch);
	SDL_DestroyTexture(m_pBGTexture);

	Mix_FreeChunk(m_pWitchFireSFX);
	Mix_FreeChunk(m_pBatFireSFX);
	Mix_FreeChunk(m_pDeathSFX);
	Mix_FreeMusic(m_pSpook4MUS);
	Mix_CloseAudio();

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}