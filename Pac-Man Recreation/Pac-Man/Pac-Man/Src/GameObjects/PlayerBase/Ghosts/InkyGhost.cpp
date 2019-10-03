// Made by Donovan Colen
#include "InkyGhost.h"
#include "../../../World.h"
#include <cmath>

InkyGhost::InkyGhost(Player * player, Ghost* blinky)
    : Ghost::Ghost(player)
    , m_pBlinky(blinky)
    , m_leadAmount(2)
{
    m_isInHouse = true;
    m_canUseHouseDoor = false;
    m_moveDir = Vector2D{ 0, -1 };

}

// calculates the appropriate chase target for Inky and pathfinds to it
void InkyGhost::ChaseMode(Vector2D oppositeDir, Vector2D curLoc)
{
    Vector2D tempTarget = m_pPlayer->GetTileLocation();
    Vector2D playDir = m_pPlayer->GetMoveDir();
    
    // get the square two ahead of the player
    tempTarget.m_x += playDir.m_x * m_leadAmount;
    tempTarget.m_y += playDir.m_y * m_leadAmount;
    Vector2D blinkyLoc = m_pBlinky->GetTileLocation();

    // create a vector from blinky to the square ahead of the player
    Vector2D vector{ tempTarget.m_x - blinkyLoc.m_x, tempTarget.m_y - blinkyLoc.m_y };

    // double the length
    vector.m_x *= 2;
    vector.m_y *= 2;

    // get the tile that the vector is pointing at
    m_targetTile.m_x = blinkyLoc.m_x + vector.m_x;
    m_targetTile.m_y = blinkyLoc.m_y + vector.m_y;

    ScatterMode(oppositeDir, curLoc, m_targetTile);

}

// resets Inky to starting state
void InkyGhost::Reset(int x, int y)
{
    PlayerBase::Reset(x, y);
    m_canBeEatten = false;
    m_wasEatten = false;
    m_isInHouse = true;
    if (World::GetInstance()->GetDotEattenCount() >= s_kInkyDots)
    {
        m_canUseHouseDoor = true;
    }
    else
    {
        m_canUseHouseDoor = false;
    }

}
