// Made by Donovan Colen
#pragma once
#include <string>
#include <iostream>
#include <Application/Application.h>
#include "../../Tile.h"


class PlayerBase : public GameObject
{
protected:
    Vector2D m_quedDir;
    TextureEvent m_texture;
    float m_speed;

public:
    PlayerBase();
    virtual ~PlayerBase() = default;

    // void funcs
    void Init(int x, int y, int w, int h, float speed, std::string textureFile, std::string name);
    void SnapToTile();  // snap the object to its current tile
    void SnapToTile(int x, int y);  // snap the object to the tile at the give position
    void Teleport();    // checks to see if object is in tunnel and teleport if it is
    virtual void Reset(int x, int y);	// resets back to starting position and settings

    const Vector2D GetTileLocation();   // returns the tile location of the object based of the objects center point
    const Vector2D GetPixelLocation();  // returns the pixel location of the object based of the objects center point
    bool IsInTurnTolerance();   // check if object is in the turning tolerance
};