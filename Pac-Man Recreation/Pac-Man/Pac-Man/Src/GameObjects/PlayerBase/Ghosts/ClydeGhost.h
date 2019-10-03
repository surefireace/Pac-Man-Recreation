// Made by Donovan Colen
#pragma once
#include "Ghost.h"

class ClydeGhost : public Ghost
{
private:
    int m_distance;

public:
    ClydeGhost(Player* player);

    virtual void ChaseMode(Vector2D oppositeDir, Vector2D curLoc) override;	// calculates the appropriate chase target and pathfinds to it
    virtual void Reset(int x, int y) override;	// resets the ghost to starting state
};