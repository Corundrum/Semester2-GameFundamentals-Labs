#include <iostream>
#include "States.h"
#include "StateManager.h"
#include "EventManager.h"
#include "TextureManager.h"
#include "CollisionManager.h"
#include "Engine.h"
#include "tinyxml2.h"

using namespace std;

void State::Render()
{
	SDL_RenderPresent(Engine::Instance().GetRenderer());
}

TitleState::TitleState(){}

void TitleState::Enter()
{
	
}

void TitleState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_N))
	{
		
		STMA::ChangeState( new GameState() );
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
	
}

PauseState::PauseState(){}

void PauseState::Enter()
{
	
}

void PauseState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_R))
		STMA::PopState();
}

void PauseState::Render()
{
	// First render the GameState.
	STMA::GetStates().front()->Render();
	// Now render the rest of PauseState.
	SDL_SetRenderDrawBlendMode(Engine::Instance().GetRenderer(), SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 255, 255, 128);
	SDL_Rect rect = { 255, 128, 512, 512 };
	SDL_RenderFillRect(Engine::Instance().GetRenderer(), &rect);
	State::Render();
}

void PauseState::Exit()
{
	
}

void GameState::ClearTurrets()
{
	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		delete m_turrets[i];
		m_turrets[i] = nullptr;
	}
	m_turrets.clear();
	m_turrets.shrink_to_fit();
}

void GameState::SaveState()
{
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.DeleteChildren();
	tinyxml2::XMLNode* pRoot = xmlDoc.NewElement("Root");
	xmlDoc.InsertEndChild(pRoot);

	for (unsigned i = 0; i < m_turrets.size(); i++)
	{
		tinyxml2::XMLElement* pElement = xmlDoc.NewElement("Turret");
		pElement->SetAttribute("x", m_turrets[i]->m_dst.x);
		pElement->SetAttribute("y", m_turrets[i]->m_dst.y);
		pRoot->InsertEndChild(pElement);
	}
	xmlDoc.SaveFile("TurretSavedData.xml");
}

void GameState::loadState()
{
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile("TurretSavedData.xml");
	tinyxml2::XMLNode* pRoot = xmlDoc.FirstChildElement("Root");
	tinyxml2::XMLElement* pElement = pRoot->FirstChildElement("Turret");
	while (pElement != nullptr)
	{
		int x, y;
		pElement->QueryIntAttribute("x", &x);
		pElement->QueryIntAttribute("y", &y);
		Turret* temp = new Turret({x, y, 100, 100});
		m_turrets.push_back(temp);
		pElement = pElement->NextSiblingElement("Turret");
	}

}

GameState::GameState():m_spawnCtr(0) {}

void GameState::Enter()
{
	TEMA::Load("Img/Turret.png", "turret");
	TEMA::Load("Img/Enemies.png", "enemy");
	s_enemies.push_back(new Enemy({512, -200, 40, 57}));
	loadState();
}

void GameState::Update()
{
	if (EVMA::KeyPressed(SDL_SCANCODE_P))
	{
		STMA::PushState(new PauseState());
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_T))
	{
		m_turrets.push_back(new Turret({ 50,618,100,100 }));
	}
	if (EVMA::KeyPressed(SDL_SCANCODE_C))
	{
		ClearTurrets();
	}
	if (m_spawnCtr++ % 180 == 0)
		s_enemies.push_back(new Enemy({ rand() % (1024 - 40), -57, 40, 57 }));
	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Update();
	for (unsigned i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Update();
	for (unsigned i = 0; i < s_bullets.size(); i++)
		s_bullets[i]->Update();

	cout << s_bullets.size() << endl;

	// Cleanup bullets and enemies that go off screen.
	//enemies
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		if (s_enemies[i]->GetPos().y > HEIGHT)
		{
			delete s_enemies[i];
			s_enemies[i] = nullptr;
			s_enemies.erase(s_enemies.begin() + i);
			s_enemies.shrink_to_fit();
			break;
		}
	}
	//bullets
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		if (s_bullets[i]->getDst().x < 0 || s_bullets[i]->getDst().x > WIDTH || s_bullets[i]->getDst().y < 0 || s_bullets[i]->getDst().y > HEIGHT)
		{
			delete s_bullets[i];
			s_bullets[i] = nullptr;
			s_bullets.erase(s_bullets.begin() + i);
			s_bullets.shrink_to_fit();
		}
	}

	// Check for collisions with bullets and enemies.
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		for (unsigned j = 0; j < s_enemies.size(); j++)
		{
			if (CollisionManager::AABBCheck(s_enemies[j]->getDst(), s_bullets[i]->getDst()))
			{
				delete s_bullets[i];
				s_bullets[i] = nullptr;
				s_bullets.erase(s_bullets.begin() + i);
				s_bullets.shrink_to_fit();
				s_enemies[j]->Hits()++;

				if (s_enemies[j]->Hits() >= 4)
				{
					delete s_enemies[j];
					s_enemies[j] = nullptr;
					s_enemies.erase(s_enemies.begin() + j);
					s_enemies.shrink_to_fit();
					break;
				}

				break;
			}
		}
	}
}

void GameState::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 0, 0, 0, 255);
	SDL_RenderClear(Engine::Instance().GetRenderer());

	for (unsigned i = 0; i < m_turrets.size(); i++)
		m_turrets[i]->Render();
	for (unsigned i = 0; i < s_enemies.size(); i++)
		s_enemies[i]->Render();
	for (unsigned i = 0; i < s_bullets.size(); i++)
		s_bullets[i]->Render();

	SDL_Rect spawnBox = { 50, 618, 100, 100 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 255, 255, 255, 255);
	SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &spawnBox);

	// This code below prevents SDL_RenderPresent from running twice in one frame.
	if ( dynamic_cast<GameState*>( STMA::GetStates().back() ) ) // If current state is GameState.
		State::Render();
}

void GameState::Exit()
{
	SaveState();

	ClearTurrets();
	for (unsigned i = 0; i < s_enemies.size(); i++)
	{
		delete s_enemies[i];
		s_enemies[i] = nullptr;
	}
	s_enemies.clear();
	s_enemies.shrink_to_fit();
	for (unsigned i = 0; i < s_bullets.size(); i++)
	{
		delete s_bullets[i];
		s_bullets[i] = nullptr;
	}
	s_bullets.clear();
	s_bullets.shrink_to_fit();
}

void GameState::Resume()
{
	
}

// This is how static properties are allocated.
std::vector<Bullet*> GameState::s_bullets;
std::vector<Enemy*> GameState::s_enemies;