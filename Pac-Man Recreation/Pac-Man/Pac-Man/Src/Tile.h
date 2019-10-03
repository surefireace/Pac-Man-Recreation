// Made by Donovan Colen
#pragma once
#include "GameObjects/Dots.h"

class Tile
{
private:
    int m_x;        // the x position for the tile in tile location
    int m_y;        // the y position for the tile in tile location
    int m_id;       // the id for the tile used for path finding
    Dots* m_tileDot;    // pointer to the dot it hold nullptr if none

public:
    Tile();
    Tile(int x, int y, int id);
    ~Tile() = default;

    void AddDot(Dots* dot); // adds a dot to the tile

    bool HasDot();  // returns true if it has a dot
    int DotEatten();    // returns the score based on the dot the tile holds
    int GetX() { return m_x; }  // returns the tile x pos in tile space
    int GetY() { return m_y; }  // returns the tile y pos in tile space
    int GetId() { return m_id; }    // returns the tiles id
    const Vector2D GetPixelLocation();  // returns the pixel location of the tile

};