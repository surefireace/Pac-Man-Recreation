// Made by Donovan Colen
#include "PlayerBase.h"
#include "../../World.h"

PlayerBase::PlayerBase()
    : m_quedDir(Vector2D{ 0, 0 })
{
    m_moveDir = Vector2D{ -1, 0 };
}

void PlayerBase::Init(int x, int y, int w, int h, float speed, std::string textureFile, std::string name)
{
    Game::Get()->InitGameObject(this);

    m_speed = speed;
    
    //Set its attributes 
    m_texture.m_type = Event::Type::kLoadTexture; // set the type
    m_texture.m_textureName = std::move(name); // name of the texture
    m_texture.m_textureFile = std::move(textureFile); // filepath of texture
    m_texture.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&m_texture);

    //Now change the type of the event and set the game object that will have the texture
    this->AttachComponent(ComponentPool::Type::kTexture); // attach texture component
    m_texture.m_type = Event::Type::kRequestTexture;
    m_texture.m_pGameObject = this;

    //Broadcast it! After broadcasting the event structure now holds a pointer to the texture.
    EventSystem::BroadcastEvent(&m_texture);

    this->SetObjName(name);
    this->SetSize(Vector2D{ w * kPixelsPerMeter, h * kPixelsPerMeter });
    this->SetPosition(Vector2D{ x * kPixelsPerMeter, y * kPixelsPerMeter });
    this->SetMoveSpeed(speed);

    this->AttachComponent(ComponentPool::Type::kMove);
    m_isRendered = true;
    View::Get()->AddRenderable(this);
}

// returns the tile location of the object based of the objects center point
const Vector2D PlayerBase::GetTileLocation()
{
    // take the position convert to pixel space and adjust it to the center of the object
    // then divide it by the tile size to find the objects location in tile space
    Vector2D temp{ ((m_position.m_x * kMetersPerPixel) + ((m_size.m_x * kMetersPerPixel) / 2)) / s_kTileSize,
                   ((m_position.m_y * kMetersPerPixel) + ((m_size.m_y * kMetersPerPixel) / 2)) / s_kTileSize };

    // floor the values because partial tiles will mess with finding the correct tile
    temp.m_x = std::floorf(temp.m_x);
    temp.m_y = std::floorf(temp.m_y);
    return temp;
}

// returns the pixel location of the object based of the objects center point
const Vector2D PlayerBase::GetPixelLocation()
{
    // take the position convert to pixel space and adjust it to the center of the object
    Vector2D temp{ ((m_position.m_x * kMetersPerPixel) + ((m_size.m_x * kMetersPerPixel) / 2)),
                   ((m_position.m_y * kMetersPerPixel) + ((m_size.m_y * kMetersPerPixel) / 2))};

    return temp;
}

// snap the object to its current tile
void PlayerBase::SnapToTile()
{
    Vector2D curLoc = GetTileLocation();
    Tile* currTile = World::GetInstance()->GetTile((int)curLoc.m_x, (int)curLoc.m_y);
    Vector2D tilePixel = currTile->GetPixelLocation();
    SetPosition(Vector2D{ (tilePixel.m_x * kPixelsPerMeter) - (m_size.m_x / 2),
                          (tilePixel.m_y * kPixelsPerMeter) - (m_size.m_y / 2) });
}

// check if object is in the turning tolerance
bool PlayerBase::IsInTurnTolerance()
{
    Vector2D player = GetPixelLocation();
    Vector2D curLoc = GetTileLocation();
    Tile* currTile = World::GetInstance()->GetTile((int)curLoc.m_x, (int)curLoc.m_y);
    Vector2D tile = currTile->GetPixelLocation();

    // check to see if the player is with in snap tolerance
    if (player.m_x == tile.m_x || (player.m_x < (tile.m_x + s_kTolerance) && player.m_x >(tile.m_x - s_kTolerance)))
    {
        if (player.m_y == tile.m_y || (player.m_y < (tile.m_y + s_kTolerance) && player.m_y >(tile.m_y - s_kTolerance)))
        {
            return true;
        }
    }
    return false;
}

// snap the object to the tile at the give position
void PlayerBase::SnapToTile(int x, int y)
{
    Tile* currTile = World::GetInstance()->GetTile(x, y);
    Vector2D tilePixel = currTile->GetPixelLocation();
    SetPosition(Vector2D{ (tilePixel.m_x * kPixelsPerMeter) - (m_size.m_x / 2),
                          (tilePixel.m_y * kPixelsPerMeter) - (m_size.m_y / 2) });

}

// checks to see if object is in tunnel and teleport if it is
void PlayerBase::Teleport()
{
    // get the location
    Vector2D tileLoc = GetTileLocation();
    Tile* curTile = World::GetInstance()->GetTile((int)tileLoc.m_x, (int)tileLoc.m_y);

    // check if in the tunnel
    if (curTile->GetId() == s_kTunnelTileID)
    {
        if (curTile->GetX() <= s_kTunnelTileLX) // if entered the left
        {
            SnapToTile(s_kTunnelTileRX - 1, s_kTunnelTileY);    // warp to other side
        }
        else if (curTile->GetX() >= s_kTunnelTileRX)    // if entered the right
        {
            SnapToTile(s_kTunnelTileLX + 1, s_kTunnelTileY);    // warp to other side
        }
    }
}

// resets back to starting position and settings
void PlayerBase::Reset(int x, int y)
{
    SetPosition(Vector2D{ x * kPixelsPerMeter, y * kPixelsPerMeter });
    m_moveDir = Vector2D{ -1, 0 };
    m_quedDir = Vector2D{ 0, 0 };
    m_diagFlip = false;
    m_horzFlip = false;
    m_vertFlip = false;
    EventSystem::BroadcastEvent(&m_texture);

}
