#pragma once
// Created by Michael Smith
/// \file Game.h

#include "IUpdatable.h"
#include "../Utils/Macros.h"
#include "GameObjects\GameObject.h"
#include <vector>

// fwd declares
class KeyCommand;
class GameObject;
class ObjectPool;
class ComponentPool;

/// \class Game
/// \brief Represents game logic
class Game : public IUpdatable
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //

    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Cleans up game from memory
    void CleanUp();

    /// \brief Updates the game
    /// \param deltaTime_sec Time since last frame in seconds
    PHAZE_API virtual void Update(const float deltaTime_sec) override;

    /// \brief Binds a key input command to a delegate of this class
    /// \param commandName Name of the command to bind
    /// \param pDelegate Pointer of the delegate function to call
    // TODO: Find a way to fix clients being able to call
    PHAZE_API virtual void BindKey(const std::string& commandName, void(Game::*pDelegate)(KeyCommand*));
private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// \brief Game playing flag
    bool m_playingFlag;

    /// \brief Game Object pool.
    ObjectPool* m_pObjPool;

    /// \brief Component pool.
    ComponentPool* m_pComponentPool;

    /// \brief Vector table of game objects this Game is in charge of.
    std::vector<IUpdatable*> m_updatables;

    /// \brief Refernce to the instance
    static Game* m_pInstance;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //


    /// \brief Constructor.
    Game();

    /// \brief Destructor.
    ~Game();

public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// \brief Sets playing flag to false on key press
    /// \param pCommand The key command bound to this function
    PHAZE_API void QuitGameByKey(KeyCommand* pCommand);

    /// \brief Adds an updatable to the list
    /// \param pUpdatable the updatable to add
    PHAZE_API void AddUpdatable(IUpdatable* pUpdatable) { if (pUpdatable) m_updatables.emplace_back(pUpdatable); }

    /// \brief Sets playing flag to false when called
    void QuitGame() { m_playingFlag = false; }

    /// \brief Returns playing flag
    const bool GetPlayingFlag() { return m_playingFlag; }

    /// \brief Initalizes a game object
    /// \param The game object to initialize
    PHAZE_API void InitGameObject(GameObject* const pObj);

    /// \brief Returns the instance of Game
    /// \return Returns the instance
    static PHAZE_API Game* Get();

    /// \brief Destroys the instance of Game
    static void Destroy();

    /// \brief Sets the game object pool
    /// \param pObjPool The game object pool to set
    void SetObjPool(ObjectPool* pObjPool) { m_pObjPool = pObjPool; }

    /// \brief Sets the component pool
    /// \param pCompPool The component pool to set
    void SetComponentPool(ComponentPool* pCompPool) { m_pComponentPool = pCompPool; }
};