#include "MovementComponent.h"
#include "GameObject.h"

// TODO: data drive and/or grab from game object
//const float kPixelsPerSecond = 100.f;

void MovementComponent::Update(float deltaTime_sec)
{
    // local for game object position
    Vector2D position = m_pGameObject->GetPosition();

    // local for game object move direction
    Vector2D moveDir = m_pGameObject->GetMoveDir();

    // local for game object move speed
    float speed = m_pGameObject->GetMoveSpeed();

    // calculate the position
    position += (moveDir * deltaTime_sec * speed);

    // update the game object position
    m_pGameObject->SetPosition(position);
}
