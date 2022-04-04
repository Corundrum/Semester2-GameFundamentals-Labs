#include "PlatformPlayer.h"
#include "EventManager.h"
#include "TextureManager.h"
#include <cmath>

PlatformPlayer::PlatformPlayer(SDL_Rect s, SDL_FRect d) : AnimatedSpriteObject(s, d), m_state(STATE_JUMPING), m_isGrounded(false), m_isFacingLeft(false),
m_maxVelX(9.0), m_maxVelY(JUMPFORCE), m_grav(GRAVITY), m_drag(0.85)
{
	TEMA::Load("Img/Player.png", "player");
	m_accelX = m_accelY = m_velX = m_velY = 0.0;
}

void PlatformPlayer::Update() 
{
	switch (m_state)
	{
	case STATE_IDLING:
		//Transition to Run
		if (EVMA::KeyPressed(SDL_SCANCODE_A) || EVMA::KeyPressed(SDL_SCANCODE_D))
		{
			m_state = STATE_RUNNING;
			SetAnimation(4, 0, 4);
		}
		//Transition to Jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			SetAnimation(4, 8, 9);
		}
		break;
	case STATE_RUNNING:
		//Move on ground
		if (EVMA::KeyHeld(SDL_SCANCODE_A))
		{
			m_accelX = -1.5;
			if (!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}
		//Transition to Idle
		if (!EVMA::KeyHeld(SDL_SCANCODE_A) && !EVMA::KeyHeld(SDL_SCANCODE_D))
		{
			m_state = STATE_IDLING;
			SetAnimation(4, 0, 1);
		}
		//Transition to Jump
		else if (EVMA::KeyPressed(SDL_SCANCODE_SPACE) && m_isGrounded)
		{
			m_accelY = -JUMPFORCE;
			m_isGrounded = false;
			m_state = STATE_JUMPING;
			SetAnimation(4, 8, 9);
		}
		break;
	case STATE_JUMPING:
		//move in mid air
		if (EVMA::KeyHeld(SDL_SCANCODE_A) && m_dst.x > 0)
		{
			m_accelX = -1.5;
			if(!m_isFacingLeft)
				m_isFacingLeft = true;
		}
		else if (EVMA::KeyHeld(SDL_SCANCODE_D) && m_dst.x > WIDTH - m_dst.w)
		{
			m_accelX = 1.5;
			if (m_isFacingLeft)
				m_isFacingLeft = false;
		}
		//transition to run
		if (m_isGrounded)
		{
			m_state = STATE_RUNNING;
			SetAnimation(4, 0, 4);
		}
		break;
	}

	//PLAYER MOVEMENT
	m_velX += m_accelX;
	m_velX *= (m_isGrounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_dst.x += (float)m_velX;
	//Wrap the Player on screen
	
	m_velY += m_accelY + m_grav;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), m_maxVelY);
	m_dst.y += (float)m_velY;

	m_accelX = m_accelY = 0;
	Animate();
}

void PlatformPlayer::Render() 
{ 
	if (m_isFacingLeft)
	{
		SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst, 0, nullptr, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		SDL_RenderCopyExF(Engine::Instance().GetRenderer(), TEMA::GetTexture("player"), &m_src, &m_dst, 0, nullptr, SDL_FLIP_NONE);
	}
}

void PlatformPlayer::Stop() 
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() { m_velX = 0.0; }

void PlatformPlayer::StopY() { m_velY = 0.0; }

void PlatformPlayer::SetAccelX(double a) { m_accelX = a; }

void PlatformPlayer::SetAccelY(double a) { m_accelY = a; }

void PlatformPlayer::SetX(float x) { m_dst.x = x; }

void PlatformPlayer::SetY(float y) { m_dst.y = y; }

void PlatformPlayer::SetGrounded(bool g) { m_isGrounded = g; }

bool PlatformPlayer::IsGrounded() { return m_isGrounded; }

double PlatformPlayer::GetVelX() { return m_velX; }

double PlatformPlayer::GetVelY() { return m_velY; }
