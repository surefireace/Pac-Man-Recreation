// Made by Donovan Colen
#include "PinkyGhost.h"

PinkyGhost::PinkyGhost(Player * player)
    : Ghost::Ghost(player)
    , m_leadAmount(4)
{
    m_isInHouse = true;
    m_canUseHouseDoor = true;
    m_moveDir = Vector2D{ 0, -1 };
}

// calculates the appropriate chase target for Pinky and pathfinds to it
void PinkyGhost::ChaseMode(Vector2D oppositeDir, Vector2D curLoc)
{
    m_targetTile = m_pPlayer->GetTileLocation();
    Vector2D playDir = m_pPlayer->GetMoveDir();

    // get the square 4 ahead of the player
    m_targetTile.m_x += playDir.m_x * m_leadAmount;
    m_targetTile.m_y += playDir.m_y * m_leadAmount;
    ScatterMode(oppositeDir, curLoc, m_targetTile);

}

// resets Pinky to starting state
void PinkyGhost::Reset(int x, int y)
{
    PlayerBase::Reset(x, y);
    m_isInHouse = true;
    m_canUseHouseDoor = true;
    m_canBeEatten = false;
    m_wasEatten = false;

}
