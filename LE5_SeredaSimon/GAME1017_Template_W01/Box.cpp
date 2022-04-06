#include "Box.h"
#include "Engine.h"
#include "TextureManager.h"
#define SCROLLSPEED 4

Obstacle::Obstacle() : m_dst({ 0,0,0,0 }), m_src({ 0,0,0,0 }) {}
Obstacle::Obstacle(const SDL_Rect r, const SDL_Rect c) : m_dst(r), m_src(c) {}
//can add default values to parameters and not need a default constructor

void Obstacle::Render()
{
    // for this initial lab, we are just drawing rectangles.
    SDL_RenderCopy(Engine::Instance().GetRenderer(), TEMA::GetTexture("obstacles"), &m_src, &m_dst);
}

Box::Box(const SDL_Point p, bool makeSprite, const SDL_Rect r, const SDL_Rect c) : m_pos(p), m_pSprite(nullptr) // r and c are for sprite
{
    if (makeSprite)
    {
        // m_pSprite = new Obstacle[m_numSprites]; // for dynamic array.
        m_pSprite = new Obstacle(r, c);
    }
}

Box::~Box()
{
    if (m_pSprite != nullptr)
    {
        delete m_pSprite;
    }
}

Box* Box::Clone()
{
    Box* clone = new Box(this->m_pos, false); // deep copy of object
    clone->m_pSprite = new Obstacle(this->m_pSprite->m_dst, this->m_pSprite->m_src);
    return clone;
}

void Box::Update()
{
    m_pos.x -= SCROLLSPEED;
    if (m_pSprite != nullptr)
    {
        m_pSprite->m_dst.x -= SCROLLSPEED;
    }
}

void Box::Render()
{
    if (m_pSprite != nullptr)
    {
        m_pSprite->Render();
    }
    // if we want to render a border around each box...
   /* SDL_Rect dst = { m_pos.x, m_pos.y, 128, 128 };
    SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 156, 230, 200, 255);
    SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &dst);*/
}

