// Made by Donovan Colen
#include "Ghost.h"
#include "../../../World.h"
#include <cmath>


Ghost::Ghost(Player * player)
    : m_pPlayer(player)
    , m_targetTile(Vector2D{0,0})
    , m_scatterTarget(Vector2D{0,0})
    , m_canBeEatten(false)
    , m_wasEatten(false)
    , m_curMode(GhostModes::kScatter)
    , m_isInHouse(false)
    , m_canUseHouseDoor(false)
	, m_lastTile(Vector2D{0,0})
{

}

void Ghost::Update(float deltaTime_sec)
{
    Vector2D oppositeDir{ -m_moveDir.m_x, -m_moveDir.m_y };
    Vector2D curLoc = GetTileLocation();

    if (m_isInHouse && !m_canUseHouseDoor)
    {
        InHouse(oppositeDir, curLoc);
    }
    else if (m_canUseHouseDoor && !m_wasEatten)
    {
        LeaveHouse(oppositeDir, curLoc);
    }
    else
    {
        Teleport(); // check if they are in the tunnel

        if (m_quedDir == Vector2D{ 0, 0 } && m_lastTile != curLoc)
        {
            if (m_wasEatten)
            {
                m_canUseHouseDoor = true;
                ScatterMode(oppositeDir, curLoc, s_kGhostHouse);
                if (curLoc == s_kGhostHouse)
                {
                    m_wasEatten = false;
                    m_canBeEatten = false;
                    SetMoveSpeed(m_speed);
                    EventSystem::BroadcastEvent(&m_texture);

                }
            }
            else
            {
                switch (m_curMode)
                {
                case Ghost::GhostModes::kChase:
                    ChaseMode(oppositeDir, curLoc);
                    break;

                case Ghost::GhostModes::kScatter:
                    ScatterMode(oppositeDir, curLoc, m_scatterTarget);
                    break;

                case Ghost::GhostModes::kFrighten:
                    FrightenedMode(oppositeDir, curLoc);
                    break;

                case Ghost::GhostModes::kNone:
                    printf("ERROR NO MODE!!!");
                    break;

                default:
                    printf("ERROR MODE!!!");
                    break;
                }
            }

        }
        else if (m_quedDir != Vector2D{ 0, 0 })
        {
            if (IsInTurnTolerance() && m_quedDir != m_moveDir)
            {
                m_moveDir = m_quedDir;
                SnapToTile();
                m_quedDir = Vector2D{ 0, 0 };
            }
            else if (m_quedDir == m_moveDir)
            {
                m_quedDir = Vector2D{ 0, 0 };
            }

        }
    }

    GameObject::Update(deltaTime_sec);
}

// standard chase aka how blinky chases.
void Ghost::ChaseMode(Vector2D oppositeDir, Vector2D curLoc)
{
    m_targetTile = m_pPlayer->GetTileLocation();
    ScatterMode(oppositeDir, curLoc, m_targetTile);
}

// simple path find. got the direction of the tile that is closer to the target based on crow fly distance
void Ghost::ScatterMode(Vector2D oppositeDir, Vector2D curLoc, Vector2D target)
{
    std::map<Tile*, Vector2D> posibleDirs = World::GetInstance()->GetValidNeighbors((int)curLoc.m_x, (int)curLoc.m_y, m_canUseHouseDoor);
    float smallestDist = INFINITY;

    for (auto pair : posibleDirs)
    {
        if (pair.second != oppositeDir)
        {
            Vector2D tileLoc{ (float)pair.first->GetX(), (float)pair.first->GetY() };
            float dx = target.m_x - tileLoc.m_x;
            float dy = target.m_y - tileLoc.m_y;
            dx *= dx;
            dy *= dy;
            float dist = dx + dy;

            if (dist < smallestDist)
            {
                smallestDist = dist;
                m_quedDir = pair.second;
                m_lastTile = curLoc;
            }
        }
    }

}

// changes sprite and pathfinds back to ghost house
void Ghost::Eatten()
{
    if (m_wasEatten == false)   // cant be eatten twice
    {
        m_pPlayer->AddScore(200);
        EventSystem::BroadcastEvent(&m_eattenTexture);  // change texture
        m_wasEatten = true;
        SetMoveSpeed(m_speed * 2);
    }
}

// sets the mode of the ghost
void Ghost::SetMode(Ghost::GhostModes newMode)
{
    if (m_curMode != GhostModes::kFrighten)
    {
        m_quedDir = Vector2D{ -m_moveDir.m_x, -m_moveDir.m_y };
        m_lastTile = Vector2D{ 0,0 };
    }
    if(newMode == GhostModes::kFrighten && !m_wasEatten)
    {
        // change the texture
        SetMoveSpeed(m_speed / 2);
        EventSystem::BroadcastEvent(&m_afraidTexture);
        m_canBeEatten = true;
    }
    else
    {
        if (!m_wasEatten)   // if wasnt eatten switch texture back
        {
            m_canBeEatten = false;
            SetMoveSpeed(m_speed);
            EventSystem::BroadcastEvent(&m_texture);
        }
    }
    m_curMode = newMode;
}

// the behavior for when a ghost is in the house
void Ghost::InHouse(Vector2D oppositeDir, Vector2D curLoc)
{
    std::vector<Vector2D> posibleDirs = World::GetInstance()->GetValidDirections((int)curLoc.m_x, (int)curLoc.m_y);
    std::vector<Vector2D> dirChoices;
    for (Vector2D dir : posibleDirs)
    {
        if (dir != Vector2D{ 1, 0 } && dir != Vector2D{ -1, 0 })    // cant go left or right while in the house
        {
            dirChoices.emplace_back(dir);
        }
    }

    if (dirChoices.size() == 1)
    {
        m_quedDir = dirChoices[0];
    }
    else
    {
        if (dirChoices[0] != oppositeDir)
        {
            m_quedDir = dirChoices[0];
        }
        else if (dirChoices[1] != oppositeDir)
        {
            m_quedDir = dirChoices[1];
        }
    }
    m_lastTile = curLoc;

    if (m_quedDir != m_moveDir)
    {
        m_moveDir = m_quedDir;
        m_quedDir = Vector2D{ 0, 0 };
    }
    else
    {
        m_quedDir = Vector2D{ 0, 0 };
    }

}

// the behavior for how the ghost leaves the house
void Ghost::LeaveHouse(Vector2D oppositeDir, Vector2D curLoc)
{
    m_targetTile.m_x = std::floorf(s_kBlinkyStart.m_x / s_kTileSize);
    m_targetTile.m_y = std::ceilf(s_kBlinkyStart.m_y / s_kTileSize);
    if (curLoc == m_targetTile)
    {
        m_isInHouse = false;
        m_canUseHouseDoor = false;
    }
    ScatterMode(oppositeDir, curLoc, m_targetTile);

    if (m_quedDir != m_moveDir)
    {
        m_moveDir = m_quedDir;
        SnapToTile();
        m_quedDir = Vector2D{ 0, 0 };
    }
    else
    {
        m_quedDir = Vector2D{ 0, 0 };
    }

}

// sets the textures for frightened mode
void Ghost::SetFrightendModeTextures(TextureEvent afraid, TextureEvent eatten)
{
    m_afraidTexture = afraid;
    m_eattenTexture = eatten;

    m_afraidTexture.m_pGameObject = this;
    m_eattenTexture.m_pGameObject = this;
}

// resets the ghost to starting state
void Ghost::Reset(int x, int y)
{
    PlayerBase::Reset(x, y);
    m_isInHouse = false;
    m_canUseHouseDoor = false;
    m_canBeEatten = false;
    m_wasEatten = false;
}

// chooses direction based on rng
void Ghost::FrightenedMode(Vector2D oppositeDir, Vector2D curLoc)
{
    std::vector<Vector2D> posibleDirs = World::GetInstance()->GetValidDirections((int)curLoc.m_x, (int)curLoc.m_y);

    std::vector<Vector2D> dirChoices;
    for (Vector2D dir : posibleDirs)
    {
        if (dir != oppositeDir)
        {
            dirChoices.emplace_back(dir);
        }
    }
    if (dirChoices.size() > 0)
    {
        int choice = std::rand() % dirChoices.size();
        m_quedDir = dirChoices[choice];
        m_lastTile = curLoc;
    }
}
