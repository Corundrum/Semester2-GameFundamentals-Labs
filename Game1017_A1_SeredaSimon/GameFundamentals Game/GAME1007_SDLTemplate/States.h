#pragma once
#ifndef _STATES_H_
#define _STATES_H_

#include "Witch.h"
#include "Fireball.h"
#include "Bat.h"
#include <vector>
#include <SDL_mixer.h>
#include <SDL_image.h>

//STATE
class State
{
protected:
	State() = default;
	int m_MouseX, m_MouseY;
public:
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume() {};
};

//START SCENE
class MenuState : public State
{
private:
	SDL_Texture* m_pStartButton;
	SDL_Rect m_StartButtonSrc = {0, 0, 250, 100},
		m_StartButtonDst = {400, 600, 250, 100};


public:
	MenuState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

//LOSE SCENE
class LoseState : public State
{
private:
	SDL_Texture* m_pMainMenuButton;
	SDL_Rect m_pMainMenuButtonSrc = { 0, 0, 250, 100 },
		m_pMainMenuButtonDst = { 400, 600, 250, 100 };

	

public:
	LoseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

//WIN SCENE
class WinState : public State
{
private:
	SDL_Texture* m_pMainMenuButton;
	SDL_Rect m_pMainMenuButtonSrc = { 0, 0, 250, 100 },
		m_pMainMenuButtonDst = { 400, 600, 250, 100 };
public:
	WinState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

//PAUSE SCENE
class PauseState : public State
{
private:
	SDL_Texture* m_pResumeButton;
	SDL_Rect m_pResumeButtonSrc = { 0, 0, 250, 100 },
		m_pResumeButtonDst = { 400, 600, 250, 100 };
public:
	PauseState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
};

//TEST PLAY SCENE
class GameState : public State
{
private:
	//Textures
	SDL_Texture* m_pwitchTexture;
	SDL_Texture* m_pBGTexture;
	SDL_Texture* m_pFireballTexture;
	SDL_Texture* m_pBatTexture;
	//Sound
	Mix_Chunk* m_pWitchFireSFX;
	Mix_Chunk* m_pBatFireSFX;
	Mix_Chunk* m_pDeathSFX;
	Mix_Music* m_pSpook4MUS;
	bool m_paused;
	//Objects
	Witch m_witch;
	std::vector<Fireball*> m_pFireballs;
	std::vector<Bat*> m_pBats;
	std::vector<Fireball*> m_pBatfire;
	//tools
	int m_spawnCtr = 0;
	int m_spawnMax = 150;
	int m_shootCtr = 0;
	int m_shootMax = 120;
	float m_fireballTimer = 1;
	//Background
	SDL_Rect m_bg1 = { 0,0,1024,768 };
	SDL_Rect m_bg2 = { 1024,0,1024,768 };

public:
	GameState();
	virtual void Enter();
	virtual void Update();
	virtual void Render();
	virtual void Exit();
	virtual void Resume();
};
#endif