#pragma once
// Created by Michael Smith

/// \file GameObject.h

#include "../IUpdatable.h"
#include "../../Views/IRenderable.h"
#include "../Math/Vector2D.h"
#include <string>
#include <SDL_rect.h>
#include "../../Utils/Macros.h"
#include <unordered_map>
#include <map>
#include "ComponentPool.h"
#include <functional>

// fwd declares
struct SDL_Texture;
class IComponent;
struct TextureComponent;
struct lua_State;
struct TextureEvent;
struct PhysicsComponent;
class KeyCommand;

/// \class GameObject
/// \brief Represents a game object
class GameObject : public IUpdatable, public IRenderable
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Default Constructor.
    GameObject() {}
    
    /// \brief Copy Constructor.
    /// \param original The object to deep copy.
    GameObject(const GameObject& original);

    /// \brief Updates the game object
    /// \param deltaTime_sec Time since last frame in seconds
    PHAZE_API virtual void Update(float deltaTime_sec) override;

    /// \brief Renders the game object
    PHAZE_API virtual void Render() override;

    /// \brief Attaches a component to this game object.
    /// \param type Type of component to attach
    /// \param bodyType Type of rigid body to create (for physics component only!)
    PHAZE_API void AttachComponent(const ComponentPool::Type& type);

    /// \brief Detaches a component from this game object.
    /// \param ID of component to delete.
    PHAZE_API void DettachComponent(const unsigned int componentID);
protected:
    /// \brief Object name
    std::string m_objName;

    /// \brief Object position
    Vector2D m_position;

    /// \brief Object size
    Vector2D m_size;

    /// \brief Object movement direction
    Vector2D m_moveDir;

    /// \brief Object sdl texture
    SDL_Texture* m_pTexture;

    /// \brief Movement speed for this game object
    float m_moveSpeed;

    /// Component pool.
    ComponentPool* m_pComponentPool;

    /// \brief Moving right flag
    bool m_moveRightFlag = false;

    /// \brief Moving up flag
    bool m_moveUpFlag = false;

    /// \brief Moving left flag
    bool m_moveLeftFlag = false;

    /// \brief Moving down flag
    bool m_moveDownFlag = false;

    /// Map of components attached to this game object.
    std::unordered_multimap<unsigned int, IComponent*> m_components;

    // TODO: Make this systematic somehow
    /// Static ID for each game object
    unsigned int m_id;

    /// \brief Lua state
    lua_State* m_pLuaState;

private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //

public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// \brief Returns position
    Vector2D GetPosition() { return m_position; }

    /// \brief Sets the position
    /// \param newPos Position to be set
    PHAZE_API void SetPosition(Vector2D newPos) { m_position = newPos; }

    /// \brief Returns size
    Vector2D GetSize() { return m_size; }

    /// \brief Sets the size
    /// \param newSize Size to be set
    PHAZE_API void SetSize(Vector2D newSize) { m_size = newSize; }

    /// \brief Returns move speed
    float GetMoveSpeed() { return m_moveSpeed; }

    /// \brief Sets the move speed for this object
    /// \param newSpeed The new speed for the object
    PHAZE_API void SetMoveSpeed(float newSpeed) { m_moveSpeed = newSpeed; }

    /// \brief Returns movement direction
    Vector2D GetMoveDir() { return m_moveDir; }

    /// \brief Sets the movement direction
    /// \param newDir Movement direction to be set
    void SetMoveDir(Vector2D newDir) { m_moveDir = newDir; }

    /// \brief Returns object name
    std::string GetObjName() { return m_objName; }

    /// \brief Sets the name
    /// \param newDir Name to be set
    void SetObjName(std::string newName) { m_objName = newName; }

    /// \brief Returns the sdl texture
    // TODO: Make a templated function EX: GetComponent<T>
    //TextureComponent* GetTextureComponent();

    /// \brief Returns the physics component
    // TODO: Make a templated function EX: GetComponent<T>
    PhysicsComponent* GetPhysicsComponent();

    /// \brief Sets the component pool to use.
    /// \param pNewPool The pool to use
    void SetComponentPool(ComponentPool* pNewPool) { m_pComponentPool = pNewPool; }

    /// Returns Object ID.
    unsigned int GetID() { return m_id; }

    /// Sets the Object ID.
    void SetID(unsigned int newID) { m_id = newID; }

    /// \brief Attaches the lua state
    void AttachToLuaTable();

    /// \brief attaches to lua state
    void AttachLuaState(lua_State* pLuaState) { if (pLuaState) m_pLuaState = pLuaState; }
};