// Made by Donovan Colen
#include "Tile.h"
#include "World.h"

Tile::Tile()
    : m_x(0)
    , m_y(0)
    , m_id(0)
    , m_tileDot(nullptr)
{
}

Tile::Tile(int x, int y, int id)
    : m_x(x)
    , m_y(y)
    , m_id(id)
    , m_tileDot(nullptr)
{
}

// adds a dot to the tile
void Tile::AddDot(Dots * dot)
{
    m_tileDot = dot;
}

// returns true if it has a dot
bool Tile::HasDot()
{
    if (m_tileDot != nullptr)
    {
        return true;
    }
    return false;
}

// returns the score the dot gives. toggles off the dot from being rendered or eaten twice
int Tile::DotEatten()
{
    return m_tileDot->Eat();
}

// convert to pixel space and ajust to the center of the tile
const Vector2D Tile::GetPixelLocation()
{
    Vector2D temp{ (float)(m_x * s_kTileSize) + (s_kTileSize / 2), (float)(m_y * s_kTileSize) + (s_kTileSize / 2) };
    return temp;
}
