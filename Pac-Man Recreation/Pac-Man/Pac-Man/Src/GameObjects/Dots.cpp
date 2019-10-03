// Made by Donovan Colen
#include "Dots.h"
#include "../World.h"

Dots::Dots()
    : m_eatten(false)
{

}

void Dots::Init(int x, int y, int w, int h, float speed, TextureEvent textureEvent, std::string name)
{
    Game::Get()->InitGameObject(this);

    //Now change the type of the event and set the game object that will have the texture
    this->AttachComponent(ComponentPool::Type::kTexture); // attach texture component
    textureEvent.m_type = Event::Type::kRequestTexture;
    textureEvent.m_pGameObject = this;

    //Broadcast it! After broadcasting the event structure now holds a pointer to the texture.
    EventSystem::BroadcastEvent(&textureEvent);

    this->SetSize(Vector2D{ w * kPixelsPerMeter, h * kPixelsPerMeter });
    this->SetPosition(Vector2D{ x * kPixelsPerMeter, y * kPixelsPerMeter });
    this->SetMoveSpeed(speed);

    this->AttachComponent(ComponentPool::Type::kMove);
    m_isRendered = true;
    View::Get()->AddRenderable(this);
}

// returns the amount of score it is worth as well as turns rendering and colision
int Dots::Eat()
{
    if (!m_eatten)
    {
        World::GetInstance()->DotEatten();
        m_eatten = true;
        m_isRendered = false;
        return 10;
    }
    return 0;
}

void Dots::Reset()
{
    m_eatten = false;
    m_isRendered = true;
}
