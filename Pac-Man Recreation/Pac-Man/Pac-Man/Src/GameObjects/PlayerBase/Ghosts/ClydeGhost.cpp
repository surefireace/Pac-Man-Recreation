// Made by Donovan Colen
#include "ClydeGhost.h"
#include "../../../World.h"

ClydeGhost::ClydeGhost(Player * player)
    : Ghost::Ghost(player)
    , m_distance(8)
{
    m_isInHouse = true;
    m_canUseHouseDoor = false;
    m_moveDir = Vector2D{ 0, -1 };

}

// calculates the appropriate chase target for Clyde and pathfinds to it
void ClydeGhost::ChaseMode(Vector2D oppositeDir, Vector2D curLoc)
{
    Vector2D playerTile = m_pPlayer->GetTileLocation();
    float dx = playerTile.m_x - curLoc.m_x;
    float dy = playerTile.m_y - curLoc.m_y;
    dx *= dx;
    dy *= dy;
    float dist = dx + dy;

    // check to see if outside the distance. note dist is squared so save performance
    if (dist >= (m_distance * m_distance))
    {
        m_targetTile = playerTile;
        ScatterMode(oppositeDir, curLoc, m_targetTile);

    }
    else
    {
        ScatterMode(oppositeDir, curLoc, m_scatterTarget);
    }
    
}

// resets Clyde to starting state
void ClydeGhost::Reset(int x, int y)
{
    PlayerBase::Reset(x, y);
    m_canBeEatten = false;
    m_wasEatten = false;
    m_isInHouse = true;
    if (World::GetInstance()->GetDotEattenCount() >= s_kClydeDots)
    {
        m_canUseHouseDoor = true;
    }
    else
    {
        m_canUseHouseDoor = false;
    }
}
