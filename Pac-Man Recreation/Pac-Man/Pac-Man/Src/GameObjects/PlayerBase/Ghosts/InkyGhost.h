// Made by Donovan Colen
#pragma once
#include "Ghost.h"

class InkyGhost : public Ghost
{
private:
    int m_leadAmount;   // the amount that the target tile is ahead of the player 
    Ghost* m_pBlinky;   // for target calcualtions

public:
    InkyGhost(Player* player, Ghost* blinky);

    virtual void ChaseMode(Vector2D oppositeDir, Vector2D curLoc) override;	// calculates the appropriate chase target for Inky and pathfinds to it
    virtual void Reset(int x, int y) override;	// resets Inky to starting state
};