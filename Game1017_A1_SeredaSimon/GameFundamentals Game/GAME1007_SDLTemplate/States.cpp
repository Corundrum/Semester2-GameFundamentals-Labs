#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>

//STATE
void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}


//START STATE
MenuState::MenuState() {}

void MenuState::Enter()
{
	std::cout << "entering Menu State..." << std::endl;
}

void MenuState::Update()
{

}

void MenuState::Render()
{

	State::Render();
}

void MenuState::Exit()
{
	std::cout << "Exiting Start State..." << std::endl;
}


//PAUSE MENU
PauseState::PauseState() {}

void PauseState::Enter()
{
	std::cout << "entering Pause State..." << std::endl;
}

void PauseState::Update()
{

}

void PauseState::Render()
{
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void PauseState::Exit()
{
	std::cout << "exiting Pause State..." << std::endl;
}


//TEST PLAY SCENE
GameState::GameState() {}

void GameState::Enter()
{
	std::cout << "entering Test Play State..." << std::endl;
	//INITIALIZE TEXTURES
	if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) != 0)
	{
		m_pwitchTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "witch.png");
		m_pBGTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "sky_clouds.jpg");
		m_pFireballTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Fireball.png");
		m_pBatTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "bat.png");
	}
	else
	{
		std::cout << IMG_GetError() << std::endl;
	}
	//INITIALIZE SOUNDS
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
	else
	{
		std::cout << "Could not load sounds" << std::endl;
	}
	//PLAY MUSIC
	Mix_VolumeMusic(16);
	Mix_Volume(-1, 32);
	Mix_PlayMusic(m_pSpook4MUS, -1);

}

void GameState::Update()
{
	//MOVEMENT
	if (Engine::Instance().KeyDown(SDL_SCANCODE_W) && m_witch.m_dst.y > 0)
	{
		m_witch.m_dst.y -= SPEED;
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_A) && m_witch.m_dst.x > 0)
	{
		m_witch.m_dst.x -= SPEED;
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_S) && m_witch.m_dst.y < (HEIGHT - m_witch.m_dst.h))
	{
		m_witch.m_dst.y += SPEED;
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_D) && m_witch.m_dst.x < (WIDTH / 2 - m_witch.m_dst.w))
	{
		m_witch.m_dst.x += SPEED;
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_SPACE) && m_fireballTimer >= 1)
	{
		Mix_PlayChannel(-1, m_pWitchFireSFX, 0);
		m_pFireballs.push_back(new Fireball((m_witch.m_dst.x + 64), (m_witch.m_dst.y + 64)));
		m_pFireballs.shrink_to_fit();
		m_fireballTimer = 0.5;
	}

	std::cout << m_fireballTimer;

	if (m_fireballTimer < 1)
	{
		m_fireballTimer += Engine::Instance().GetDeltaTime();
	}

	//FIREBALL MOVEMENT
	for (unsigned i = 0; i < m_pFireballs.size(); i++)
	{
		m_pFireballs[i]->Update();
	}
	//FIREBALL OUT OF BOUNDS
	for (unsigned i = 0; i < m_pFireballs.size(); i++)
	{
		if (m_pFireballs[i]->m_dst.x >= 1024)
		{
			delete m_pFireballs[i];
			m_pFireballs[i] = nullptr;
			m_pFireballs.erase(m_pFireballs.begin() + i);
			m_pFireballs.shrink_to_fit();

			break;
		}
	}

	//SPAWN ENEMIES
	if (m_spawnCtr++ == m_spawnMax)
	{
		m_spawnCtr = 0;
		m_pBats.push_back(new Bat(1024, (rand() % 680)));
		m_pBats.shrink_to_fit();
	}
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		m_pBats[i]->Update();
	}
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		if (m_pBats[i]->m_dst.x <= -m_pBats[i]->m_dst.w)
		{
			delete m_pBats[i];
			m_pBats[i] = nullptr;
			m_pBats.erase(m_pBats.begin() + i);
			m_pBats.shrink_to_fit();

			break;
		}
	}

	//ENEMY BATS SHOOTING
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		if (m_shootCtr++ == m_shootMax)
		{
			m_shootCtr = 0;
			Mix_PlayChannel(-1, m_pBatFireSFX, 0);
			m_pBatfire.push_back(new Fireball(m_pBats[i]->m_dst.x, (m_pBats[i]->m_dst.y + 12)));
			m_pBatfire.shrink_to_fit();
		}
	}
	for (unsigned i = 0; i < m_pBatfire.size(); i++)
	{
		m_pBatfire[i]->UpdateReverse();
	}
	for (unsigned i = 0; i < m_pBatfire.size(); i++)
	{
		if (m_pBatfire[i]->m_dst.x <= -m_pBatfire[i]->m_dst.w)
		{
			delete m_pBatfire[i];
			m_pBatfire[i] = nullptr;
			m_pBatfire.erase(m_pBatfire.begin() + i);
			m_pBatfire.shrink_to_fit();

			break;
		}
	}

	//COLLISION FIREBALL TO BATS
	for (unsigned i = 0; i < m_pFireballs.size(); i++)
	{
		for (unsigned j = 0; j < m_pBats.size(); j++)
		{
			if (SDL_HasIntersection(&m_pFireballs[i]->m_dst, &m_pBats[j]->m_dst))
			{
				Mix_PlayChannel(-1, m_pDeathSFX, 0);
				delete m_pFireballs[i];
				delete m_pBats[j];
				m_pFireballs[i] = nullptr;
				m_pBats[j] = nullptr;
				m_pFireballs.erase(m_pFireballs.begin() + i);
				m_pBats.erase(m_pBats.begin() + j);
				m_pFireballs.shrink_to_fit();
				m_pBats.shrink_to_fit();
				break;
			}
		}
	}
	//COLLISION BAT FIREBALLS TO PLAYER
	for (unsigned i = 0; i < m_pBatfire.size(); i++)
	{
		if (SDL_HasIntersection(&m_pBatfire[i]->m_dst, &m_witch.m_dst))
		{
			Mix_PlayChannel(-1, m_pDeathSFX, 0);
			delete m_pBatfire[i];
			m_pBatfire[i] = nullptr;
			m_pBatfire.erase(m_pBatfire.begin() + i);
			m_pBatfire.shrink_to_fit();
			//delete m_pwitch;
			SDL_DestroyTexture(m_pwitchTexture);
			break;
		}
	}
	//COLLISION BATS TO PLAYER
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		if (SDL_HasIntersection(&m_pBats[i]->m_dst, &m_witch.m_dst))
		{
			Mix_PlayChannel(-1, m_pDeathSFX, 0);
			delete m_pBats[i];
			m_pBats[i] = nullptr;
			m_pBats.erase(m_pBats.begin() + i);
			m_pBats.shrink_to_fit();
			//delete m_pwitch;
			SDL_DestroyTexture(m_pwitchTexture);
			break;
		}
	}

	//SCROLLING BACKGROUND
	m_bg1.x -= SPEED;
	m_bg2.x -= SPEED;

	if (m_bg1.x <= -m_bg1.w)
	{
		m_bg1.x = 0;
		m_bg2.x = 1024;
	}

}

void GameState::Render()
{

	//Backgrounds
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg1);
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBGTexture, NULL, &m_bg2);
	//Witch
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pwitchTexture, &m_witch.m_src, &m_witch.m_dst);
	//Fireballs
	for (unsigned i = 0; i < m_pFireballs.size(); i++)
	{
		SDL_SetTextureColorMod(m_pFireballTexture, 255, 255, 255);
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pFireballTexture, &(m_pFireballs[i]->m_src), &(m_pFireballs[i]->m_dst));
	}
	//bats
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pBatTexture, &(m_pBats[i]->m_src), &(m_pBats[i]->m_dst));
	}
	//Enemy Fireballs
	for (unsigned i = 0; i < m_pBatfire.size(); i++)
	{
		SDL_SetTextureColorMod(m_pFireballTexture, 0, 255, 255);
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pFireballTexture, &(m_pBatfire[i]->m_src), &(m_pBatfire[i]->m_dst), NULL, NULL, SDL_FLIP_HORIZONTAL);
	}
}

void GameState::Exit()
{
	std::cout << "Exiting Test Play State..." << std::endl;
	//CLEAN FIREBALLS
	for (unsigned i = 0; i < m_pFireballs.size(); i++)
	{
		delete m_pFireballs[i];
		m_pFireballs[i] = nullptr;
	}
	m_pFireballs.clear();
	m_pFireballs.shrink_to_fit();
	//CLEAN BATS
	for (unsigned i = 0; i < m_pBats.size(); i++)
	{
		delete m_pBats[i];
		m_pBats[i] = nullptr;
	}
	m_pBats.clear();
	m_pBats.shrink_to_fit();
	//CLEAN BAT FIREBALLS
	for (unsigned i = 0; i < m_pBatfire.size(); i++)
	{
		delete m_pBatfire[i];
		m_pBatfire[i] = nullptr;
	}
	m_pBatfire.clear();
	m_pBatfire.shrink_to_fit();

	//DESTROY TEXTURES
	SDL_DestroyTexture(m_pwitchTexture);
	SDL_DestroyTexture(m_pBGTexture);

	//FREE MUSIC AND SFX
	Mix_FreeChunk(m_pWitchFireSFX);
	Mix_FreeChunk(m_pBatFireSFX);
	Mix_FreeChunk(m_pDeathSFX);
	Mix_FreeMusic(m_pSpook4MUS);

}

void GameState::Resume()
{
	std::cout << "Resuming Test Play State..." << std::endl;
}