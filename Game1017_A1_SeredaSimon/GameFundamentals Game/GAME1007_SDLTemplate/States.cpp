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

}

void GameState::Update()
{

}

void GameState::Render()
{

}

void GameState::Exit()
{
	std::cout << "Exiting Test Play State..." << std::endl;
}

void GameState::Resume()
{
	std::cout << "Resuming Test Play State..." << std::endl;
}