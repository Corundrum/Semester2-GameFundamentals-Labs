#include "States.h"
#include "StateManager.h"
#include "Engine.h"
#include <iostream>
using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}


TitleState::TitleState(){}

void TitleState::Enter()
{
	cout << "entering TitleState..." << endl;
	m_music.emplace("FluffingADuck", Mix_LoadMUS("Fluffing-a-Duck.mp3"));
	Mix_PlayMusic(m_music["FluffingADuck"], -1);
}

void TitleState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_N))
	{
		cout << "Changing to GameState" << endl;
		STMA::ChangeState(new GameState() );
	}
}

void TitleState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 255, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	State::Render();
}

void TitleState::Exit()
{
	cout << "Exiting TitleState..." << endl;
	Mix_FreeMusic(m_music["FluffingADuck"]);
	//call Mix_FreeMusic on music track
}
////////////////////////////

PauseState::PauseState(){}

void PauseState::Enter()
{
	cout << "entering PauseState..." << endl;
}

void PauseState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		STMA::PopState();
	}
}

void PauseState::Render()
{
	//render gamestate
	STMA::GetStates().front()->Render();
	//render rest of pausestate
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	cout << "exiting PauseState..." << endl;
}
////////////////////////////

GameState::GameState(){}

void GameState::Enter()
{
	cout << "entering GameState..." << endl;
	m_music.emplace("MonkeysSpinning", Mix_LoadMUS("Monkeys-Spinning.mp3"));
	m_sounds.emplace("Magic1", Mix_LoadWAV("Magic1.wav"));
	m_sounds.emplace("Magic2", Mix_LoadWAV("Magic2.wav"));
	Mix_PlayMusic(m_music["MonkeysSpinning"], -1);
}

void GameState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_P))
	{
		cout << "Changing to PauseState" << endl;
		Mix_PauseMusic();
		STMA::PushState(new PauseState());
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_X))
	{
		cout << "Changing to EndState" << endl;
		STMA::ChangeState(new EndState() );
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_1))
	{
		cout << "playing Magic1" << endl;
		Mix_PlayChannel(-1, m_sounds["Magic1"], 0);
	}
	if (Engine::Instance().KeyDown(SDL_SCANCODE_2))
	{
		cout << "playing Magic2" << endl;
		Mix_PlayChannel(-1, m_sounds["Magic2"], 0);
	}

}

void GameState::Render()
{
	if (dynamic_cast<GameState*>(STMA::GetStates().back()))
	{
		SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 0, 255);
		SDL_RenderClear(Engine::Instance().GetRenderer());
		State::Render();
	}
}

void GameState::Exit()
{
	cout << "Exiting GameState..." << endl;
	Mix_FreeMusic(m_music["MonkeysSpinning"]);
	Mix_FreeChunk(m_sounds["Magic1"]);
	Mix_FreeChunk(m_sounds["Magic2"]);
}

void GameState::Resume()
{
	cout << "Resuming GameState..." << endl;
	Mix_ResumeMusic();
}
////////////////////////////

EndState::EndState(){}

void EndState::Enter()
{
	cout << "entering EndState..." << endl;
}

void EndState::Update()
{
	if (Engine::Instance().KeyDown(SDL_SCANCODE_R))
	{
		cout << "Changing to PauseState" << endl;
		STMA::ChangeState(new TitleState());
	}
}

void EndState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());
	if (dynamic_cast<EndState*>(STMA::GetStates().back()))
	{
		State::Render();
	}
}

void EndState::Exit()
{
	cout << "Exiting GameState..." << endl;
}
