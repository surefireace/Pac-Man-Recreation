// Made by Donovan Colen
#pragma once
#include "Ghost.h"

class PinkyGhost : public Ghost
{
private:
    int m_leadAmount;   // the amount that the target tile is ahead of the player

public:
    PinkyGhost(Player* player);

    virtual void ChaseMode(Vector2D oppositeDir, Vector2D curLoc) override;	// calculates the appropriate chase target for Pinky and pathfinds to it
    virtual void Reset(int x, int y) override;	// resets Pinky to starting state
};