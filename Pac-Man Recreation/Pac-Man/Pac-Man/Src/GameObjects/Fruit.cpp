// Made by Donovan Colen
#include "Fruit.h"
#include "../World.h"


int Fruit::Eat()
{
    // returns the amount of score it is worth as well as turns rendering and colision
    if (!m_eatten)
    {
        m_eatten = true;
        m_isRendered = false;
        return 100;
    }
    return 0;
}

// disables the fruit because it doesnt start spawned
void Fruit::Disable()
{
    m_eatten = true;
    m_isRendered = false;
}
