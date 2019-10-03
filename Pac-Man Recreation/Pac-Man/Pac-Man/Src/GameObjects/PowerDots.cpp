// Made by Donovan Colen
#include "PowerDots.h"
#include "../World.h"

// returns the amount of score it is worth as well as turns rendering and colision and triger powered up mode
int PowerDots::Eat()
{
    if (!m_eatten)
    {
        World::GetInstance()->EmpowerPacMan();
        m_eatten = true;
        m_isRendered = false;
        return 50;
    }
    return 0;
}
