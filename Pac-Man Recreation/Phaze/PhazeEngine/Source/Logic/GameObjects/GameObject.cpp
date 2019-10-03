#include "GameObject.h"
#include <SDL_rect.h>
#include "MovementComponent.h"
#include "TextureComponent.h"
#include "PhysicsComponent.h"
#include "lua.hpp" // TODO: integrate lua into architecture
#include "../Events/EventSystem.h" // HACK: only here to help demostrate understanding of component exposing to lua
#include "../../Utils/PhazeLogger.h"
#include "../Input/InputSystem.h"

// TODO: data drive
const float kPixelsPerSecond = 100.f;
const std::string kLogOutputFile = "Logs/GameObjectLog.txt";
const std::string kLuaScriptFilename = "Scripts/Lua/GameObjScript.lua";

GameObject::GameObject(const GameObject& original)
{
    // copy all attributes
    m_objName = original.m_objName;
    m_position = original.m_position;
    m_size = original.m_size;
    m_moveDir = original.m_moveDir;
    m_transform = original.m_transform;
    m_pTexture = original.m_pTexture;
    m_pComponentPool = original.m_pComponentPool;
    //m_pLuaState = original.m_pLuaState;
    
    // TODO: find a better way to handle component copy
    m_components = original.m_components;
    m_moveRightFlag = original.m_moveRightFlag;
    m_moveDownFlag = original.m_moveDownFlag;
    m_moveUpFlag = original.m_moveUpFlag;
    m_moveLeftFlag = original.m_moveLeftFlag;

}

void GameObject::Update(float deltaTime_sec)
{
    // TEST: Did we actually create a table?
    // NOTE: Commented this out to test box2d collision
    //luaL_dofile(m_pLuaState, kLuaScriptFilename.c_str()); // < load lua script
    //lua_getglobal(m_pLuaState, "Test"); // < push the function to be called onto stack
    //lua_call(m_pLuaState, 0, 0); // < call Test()
    
    // search for the movement component
    auto mapIter = m_components.find(static_cast<int>(ComponentPool::Type::kMove));

    // sanity check -> movement component exists
    if (mapIter != m_components.end())
    {
        // static cast to movement component
        MovementComponent* pMovementComponent = static_cast<MovementComponent*>(mapIter->second);

        // update movement component
        pMovementComponent->Update(deltaTime_sec);
    }

    // search for physics component
    mapIter = m_components.find(static_cast<int>(ComponentPool::Type::kPhysics));

    // sanity check -> valid physics component
    if (mapIter != m_components.end())
    {
        // static cast to physics component
        PhysicsComponent* pPhysicsComponent = static_cast<PhysicsComponent*>(mapIter->second);

        // update physics component
        //b2Vec2 debug1 = pPhysicsComponent->m_pBody->GetPosition();
        //pPhysicsComponent->m_pBody->SetTransform(b2Vec2{ m_position.m_x, m_position.m_y }, 0);
        //b2Vec2 debug2 = pPhysicsComponent->m_pBody->GetPosition();

        //float debug = pPhysicsComponent->m_pBody->GetLinearVelocity().y;

        m_position.m_x = pPhysicsComponent->m_pBody->GetPosition().x;
        m_position.m_y = pPhysicsComponent->m_pBody->GetPosition().y;
        //pPhysicsComponent->m_pBody->SetLinearVelocity(b2Vec2{ 0, 0 });
    }
}

void GameObject::Render()
{
    m_transform.x = (int)(m_position.m_x * kMetersPerPixel);
    m_transform.y = (int)(m_position.m_y * kMetersPerPixel);
    m_transform.w = (int)(m_size.m_x * kMetersPerPixel);
    m_transform.h = (int)(m_size.m_y * kMetersPerPixel);
}

// HACK: only here to demostrate exposing components to lua
int Lua_ChangeTransform(lua_State* pState)
{
    // grab parameters off stack
    void* pParam = lua_touserdata(pState, -3);
    GameObject* pObj = static_cast<GameObject*>(pParam);
    int deltaX = (int)lua_tointeger(pState, -2);
    int deltaY = (int)lua_tointeger(pState, -1);

    pObj->SetMoveDir(Vector2D{ (float)deltaX, (float)deltaY });
    lua_pop(pState, 2);
    //PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Lua moved us!");
    return 0;
}

// HACK: Only here to request texture change for lua function
static TextureEvent s_textureEvent;

// HACK: only here to demostrate exposing components to lua
int Lua_ChangeTexture(lua_State* pState)
{
    // grab parameters off stack
    std::string textureName = lua_tostring(pState, -1);

    // change texture through event system
    s_textureEvent.m_textureName = textureName;
    s_textureEvent.m_textureFile = "Assets/Sprites/deadtree.png";

    s_textureEvent.m_type = Event::Type::kLoadTexture;
    EventSystem::BroadcastEvent(&s_textureEvent);

    s_textureEvent.m_type = Event::Type::kRequestTexture;
    EventSystem::BroadcastEvent(&s_textureEvent);

    //PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Changed texture to: " + textureName);
    return 0;
}

void GameObject::AttachComponent(const ComponentPool::Type& type)
{
    // attach the component based on type
    switch (type)
    {
        case ComponentPool::Type::kMove:
        {
            // TODO: Sanity check for any copies
            MovementComponent* pMoveComponent = static_cast<MovementComponent*>(m_pComponentPool->CreateComponent(type));

            // insert into hash table
            m_components.insert({ pMoveComponent->GetID(), pMoveComponent });

            // attach game object to component
            pMoveComponent->AttachGameObject(this);

            //// register with lua
            //lua_getglobal(m_pLuaState, "gameObjects"); //< [stack: 1]
            //lua_geti(m_pLuaState, -1, m_id + 1); //< [stack: 2]

            //lua_newtable(m_pLuaState); //< [stack: 3]

            //lua_pushcfunction(m_pLuaState, Lua_ChangeTransform); //< [stack: 4]
            //lua_setfield(m_pLuaState, -2, "move"); //< [stack: 3]

            //lua_pushlightuserdata(m_pLuaState, pMoveComponent); //< [stack: 4]
            //lua_setfield(m_pLuaState, -2, "this"); //< [stack: 3]

            //lua_setfield(m_pLuaState, -2, "transform"); //< [stack: 2]
            //lua_pop(m_pLuaState, 2); //< [stack: 0] 

            break;
        }

        case ComponentPool::Type::kTexture:
        {
            // TODO: Sanity check for any copies
            m_pTextureComponent = static_cast<TextureComponent*>(m_pComponentPool->CreateComponent(type));

            // insert into hash table
            //m_components.insert({ pTextureComponent->GetID(), pTextureComponent });

            // attach game object to component
            m_pTextureComponent->AttachGameObject(this);

            // initliaze properties
            m_pTextureComponent->m_srcRect = SDL_Rect{ -1, -1, -1, -1 };
            m_pTextureComponent->m_pTexture = nullptr;

            // NOTE: Commneted out to test Box2D collision
            //lua_getglobal(m_pLuaState, "gameObjects"); //< [stack: 1]
            //lua_geti(m_pLuaState, -1, m_id + 1); //< [stack: 2]

            //// create new empty table and push onto stack [stack: 3]
            //lua_newtable(m_pLuaState);

            //// add address of texture component to lua stack [stack: 4]
            //lua_pushlightuserdata(m_pLuaState, GetTextureComponent());

            //// set the key [stack: 3]
            //lua_setfield(m_pLuaState, -2, "this");

            //// add name of texture to lua stack [stack: 4]
            //lua_pushstring(m_pLuaState, GetTextureComponent()->m_name.c_str());

            //// set the key [stack: 3]
            //lua_setfield(m_pLuaState, -2, "name");

            //// add function value [stack: 4]
            //lua_pushcfunction(m_pLuaState, Lua_ChangeTexture);

            //// add function key [stack: 3]
            //lua_setfield(m_pLuaState, -2, "change");

            //// set key of texture component [stack: 2]
            //lua_setfield(m_pLuaState, -2, "texture");

            //lua_pop(m_pLuaState, 2);

            break;
        }

        case ComponentPool::Type::kPhysics:
        {
            // TODO: Sanity check for any copies
            PhysicsComponent* pPhysicsComponent = static_cast<PhysicsComponent*>(m_pComponentPool->CreateComponent(type));

            // insert into hash table
            m_components.insert({ pPhysicsComponent->GetID(), pPhysicsComponent });

            // attach game object to component
            pPhysicsComponent->AttachGameObject(this);

            // set physics world for component
            //pPhysicsComponent->m_p

            // create physics body and fixture
            //pPhysicsComponent->CreateBody(bodyType, m_position.m_x, m_position.m_y);
            //pPhysicsComponent->CreateFixture(PhysicsSystem::PolygonShapeType::kBox, m_size.m_x, m_size.m_y);
            break;
        }

        default:
        {
            break;
        }
    }
}

void GameObject::DettachComponent(const unsigned int componentID)
{
    // find the component
    auto compIter = m_components.find(componentID);

    // sanity check -> component exists
    if (compIter != m_components.end())
    {
        // detach the component
        compIter->second->DetachGameObject();

        // free the component from the pool
        m_pComponentPool->DeleteComponent(componentID);
    }
}

PhysicsComponent* GameObject::GetPhysicsComponent()
{
    // search for the texture component
    auto mapIter = m_components.find(static_cast<int>(ComponentPool::Type::kPhysics));

    // sanity check -> texture component exists
    if (mapIter != m_components.end())
    {
        // static cast to movement component
        PhysicsComponent* pPhysicsComponent = static_cast<PhysicsComponent*>(mapIter->second);

        // return texture
        return pPhysicsComponent;
    }

    return nullptr;
}

void GameObject::AttachToLuaTable()
{

    // create new empty table and push onto stack [stack: 1]
    lua_newtable(m_pLuaState);

    // set table name and balance stack [stack: 0]
    lua_setglobal(m_pLuaState, "gameObjects"); //< gameObjects = {}

    // grab table [stack: 1]
    lua_getglobal(m_pLuaState, "gameObjects");

    // create new empty table [stack: 2]
    lua_newtable(m_pLuaState);

    // add address of this object to lua stack (value of new entry) [stack: 3]
    lua_pushlightuserdata(m_pLuaState, this);

    // set the user data key (key of new entry) [stack: 2]
    lua_setfield(m_pLuaState, -2, "this");

    // push id onto lua stack [stack: 3]
    lua_pushinteger(m_pLuaState, m_id);

    // add new entry (-2 is index of value from stack, "id" == key) [stack: 2]
    lua_setfield(m_pLuaState, -2, "id");

    // push obj name onto stack [stack: 3]
    lua_pushstring(m_pLuaState, m_objName.c_str());

    // add new entry (-2 index of value from stack, "objName" == key) [stack: 2]
    lua_setfield(m_pLuaState, -2, "objName");

    // set table index [stack: 1]
    lua_seti(m_pLuaState, -2, m_id + 1); //< gameObjects[1]

    // obeying call convention [stack: 0]
    lua_pop(m_pLuaState, 1);
}
