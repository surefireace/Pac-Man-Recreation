#pragma once
// Created by Michael Smith

// TODO: make class only able to have one instance (maybe static?)

#include <unordered_map>
#include <memory>

// fwd declares
class GameObject;
class ComponentPool;

// typedefs
typedef std::shared_ptr<GameObject> GameObjectPtr;

/// \brief Pool of GameObjects
class ObjectPool
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //


    /// \brief Creates a game object and returns a GameObjectPtr to it.
    /// \param objName The name of the new GameObject
    GameObjectPtr CreateGameObject(const std::string& objName);

    /// \brief Creates game object by performing a deep copy of an object and returns a GameObjectPtr to it.
    /// \param pObjectPrototype The game object to copy.
    GameObjectPtr CreateDeepCloneGameObject(GameObject* pObjectPrototype);

    /// \brief Frees a game object from the pool.
    /// \param objID ID of the game object to free.
    void DeleteGameObject(const unsigned int objID);

    /// \brief Initializes the object pool. Returns false if not successful.
    bool Init();

private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// Amount of total game objects allowed.
    static const int s_kGameObjectLimit;

    // TODO: Implement resource system to better pool game objects
    /// Pool of game objects.
    std::unordered_map<unsigned int, GameObjectPtr> m_gameObjects;
    

    /// \brief Component pool
    ComponentPool* m_pComponentPool;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //


public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// Returns game object limit
    int GetObjLimit() { return s_kGameObjectLimit; }

    /// \Sets the component pool game objects will use to make components
    /// \param pComponentPool The component pool to point to
    void SetComponentPool(ComponentPool* pComponentPool);
};