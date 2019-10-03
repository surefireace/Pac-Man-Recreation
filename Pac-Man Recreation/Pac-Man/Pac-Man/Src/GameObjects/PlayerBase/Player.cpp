// Made by Donovan Colen
#include "Player.h"
#include "../../World.h"

Player::Player()
    : m_score(0)
    , m_lives(2)
{
}

// binds the keys from the file to the function
void Player::BindKey(const std::string& commandName, void(Player::* pDelegate)(KeyCommand*))
{
    KeyCommand* pCommand = InputSystem::Get()->GetKeyCommandByName(commandName);
    std::function<void(KeyCommand*)> delegate = [this, pDelegate](KeyCommand* pKeyCommand) {(this->*pDelegate)(pKeyCommand); };
    pCommand->BindDelegate(delegate);
}

//sets the move dir based on input
void Player::Move(KeyCommand * pKeyCommand)
{
    if (pKeyCommand->GetName() == "MoveLeft" && pKeyCommand->GetState() == KeyCommand::State::kPressed)
    {
        m_quedDir = Vector2D{ -1, 0 };
    }
    if (pKeyCommand->GetName() == "MoveRight" && pKeyCommand->GetState() == KeyCommand::State::kPressed)
    {
        m_quedDir = Vector2D{ 1, 0 };
    }
    if (pKeyCommand->GetName() == "MoveUp" && pKeyCommand->GetState() == KeyCommand::State::kPressed)
    {
        m_quedDir = Vector2D{ 0, -1 };
    }
    if (pKeyCommand->GetName() == "MoveDown" && pKeyCommand->GetState() == KeyCommand::State::kPressed)
    {
        m_quedDir = Vector2D{ 0, 1 };
    }
}

void Player::Update(float deltaTime_sec)
{

    Vector2D curLoc = GetTileLocation();
    Teleport(); // check if he is in the tunnel
    Tile* currTile = World::GetInstance()->GetTile((int)curLoc.m_x, (int)curLoc.m_y);
    if (currTile->HasDot())
    {
        m_score += currTile->DotEatten();
        //std::printf("Score: %d\n", m_score);
    }

    if(IsValidDir(m_quedDir) == false)
    {
        if (IsValidDir(m_moveDir) == false)
        {
            if (IsInTurnTolerance())
            {
                m_moveDir = Vector2D{ 0, 0 };
                SnapToTile();
                m_quedDir = Vector2D{ 0, 0 };
            }
        }
        else
        {
            // no need to change dir
        }
    }
    else
    {
        if (IsInTurnTolerance() && m_quedDir != m_moveDir)
        {
            if (m_quedDir == Vector2D{ -1, 0 })
            {
                m_diagFlip = false;
                m_horzFlip = false;
                m_vertFlip = false;
            }
            else if (m_quedDir == Vector2D{ 1, 0 })
            {
                m_diagFlip = false;
                m_horzFlip = true;
                m_vertFlip = true;
            }
            else if (m_quedDir == Vector2D{ 0, -1 })
            {
                m_diagFlip = true;
                m_horzFlip = true;
                m_vertFlip = false;
            }
            else if (m_quedDir == Vector2D{ 0, 1 })
            {
                m_diagFlip = true;
                m_horzFlip = false;
                m_vertFlip = true;
            }

            m_moveDir = m_quedDir;
            SnapToTile();
            m_quedDir = Vector2D{ 0, 0 };
        }
        else if(m_moveDir == Vector2D{ 0, 0 })
        {
            m_moveDir = m_quedDir;
            m_quedDir = Vector2D{ 0, 0 };
        }
    }


    GameObject::Update(deltaTime_sec);

}

// returns true if the vector is a valid direction
bool Player::IsValidDir(Vector2D newDir)
{
    Vector2D curLoc = GetTileLocation();
    std::vector<Vector2D> posibleDirs = World::GetInstance()->GetValidDirections((int)curLoc.m_x, (int)curLoc.m_y);

    bool moveValid = false;
    for (Vector2D dir : posibleDirs)
    {
        if (newDir == dir)
        {
            moveValid = true;
        }
    }
    return moveValid;
}
