#include "ObjectPool.h"
#include "GameObject.h"
#include "../../Utils/PhazeLogger.h"

// TODO: make configurable (maybe scripts?)
// statics
const int ObjectPool::s_kGameObjectLimit = 2000;
const std::string kLogOutputDir = "Logs/ObjPoolLog.txt";

void ObjectPool::DeleteGameObject(const unsigned int objID)
{
    // find the game object
    auto objIter = m_gameObjects.find(objID);

    // sanity check -> object exists
    if (objIter != m_gameObjects.end())
    {
        // destroy the object
        objIter->second.~shared_ptr();
    }
}

bool ObjectPool::Init()
{
    // allocate the pool
    m_gameObjects.reserve(s_kGameObjectLimit);
    return true;
}

void ObjectPool::SetComponentPool(ComponentPool* pComponentPool)
{
    // sanity check -> valid component pool?
    if (!pComponentPool)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputDir, "Trying to set a component pool before it was created.");
    }
    m_pComponentPool = pComponentPool;
}

GameObjectPtr ObjectPool::CreateGameObject(const std::string& objName)
{
    // sanity check -> object limit
    if (m_gameObjects.size() == s_kGameObjectLimit)
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputDir, "GameObject limit of: %d reached!", s_kGameObjectLimit);

    // create the pointer
    GameObjectPtr pNewObj(new GameObject);

    // set the requested attributes
    pNewObj->SetObjName(objName);
    pNewObj->SetComponentPool(m_pComponentPool);

    // TODO: generate id somehow
    // set the object id
    pNewObj->SetID((int)m_gameObjects.size());

    // add to pool
    m_gameObjects.insert({ pNewObj->GetID() , pNewObj });

    // return the object
    return pNewObj;
}

GameObjectPtr ObjectPool::CreateDeepCloneGameObject(GameObject* pObjectPrototype)
{
    // sanity check -> object limit
    if (m_gameObjects.size() == s_kGameObjectLimit)
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputDir, "GameObject limit of: %d reached!", s_kGameObjectLimit);

    // create the pointer
    GameObjectPtr pNewObj(new GameObject(*pObjectPrototype));

    // TODO: generate id somehow
    // set the object id
    pNewObj->SetID((int)m_gameObjects.size());

    // add to pool
    m_gameObjects.insert({ pNewObj->GetID() , pNewObj });

    // return the object
    return pNewObj;
}
