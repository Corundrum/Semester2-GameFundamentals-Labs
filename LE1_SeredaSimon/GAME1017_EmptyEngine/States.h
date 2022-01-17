#pragma once
#ifndef _STATES_H_
#define _STATES_H_

class State
{
protected:
	State() = default;
public:	
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Render();
	virtual void Exit() = 0;
	virtual void Resume(){};
};

class TitleState : public State
{
	
};

class GameState : public State
{

};

#endif