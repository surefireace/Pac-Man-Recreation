#include "Game.h"
#include "GameObjects/ObjectPool.h"
#include "../Utils/PhazeLogger.h"
#include "Physics/PhysicsSystem.h"
#include "Input/InputSystem.h"
#include "Events/EventSystem.h"

// TODO: Make configurable
const std::string kLogOutputFile = "Logs/GameLog.txt";

// static defines
Game* Game::m_pInstance = nullptr;

void Game::CleanUp()
{
    // clean up game here
    //m_updatables.~vector();
}

void Game::Update(const float deltaTime_sec)
{
    
    // update all updatables
    for (IUpdatable* pUpdatable : m_updatables)
        pUpdatable->Update(deltaTime_sec);

    // update physics world
    PhysicsSystem::Get()->Update(deltaTime_sec);
}

PHAZE_API void Game::BindKey(const std::string& commandName, void(Game::*pDelegate)(KeyCommand*))
{
    KeyCommand* pCommand = InputSystem::Get()->GetKeyCommandByName(commandName);
    std::function<void(KeyCommand*)> delegate = [this, pDelegate](KeyCommand* pKeyCommand) {(this->*pDelegate)(pKeyCommand); };
    pCommand->BindDelegate(delegate);
}

Game::Game()
{
    // allocate game object vector
    m_updatables.reserve(m_pObjPool->GetObjLimit());

    // set playing flag
    m_playingFlag = true;
}

Game::~Game()
{
    //m_updatables.~vector();
}

void Game::QuitGameByKey(KeyCommand* pCommand)
{
    if (pCommand->GetName() == "QuitGame" && pCommand->GetState() == KeyCommand::State::kPressed)
        m_playingFlag = false;
}

void Game::InitGameObject(GameObject* const pObj)
{
    pObj->SetComponentPool(m_pComponentPool);
}

PHAZE_API Game* Game::Get()
{
    if (!m_pInstance)
    {
        m_pInstance = new Game();
        PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Game Logic Created!");
    }

    return m_pInstance;
}

void Game::Destroy()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}
