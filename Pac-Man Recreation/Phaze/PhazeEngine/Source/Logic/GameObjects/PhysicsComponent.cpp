#include "PhysicsComponent.h"
#include "../../Utils/PhazeLogger.h"

const char* kLogOutput = "Logs/PhysComponent";

void PhysicsComponent::CreateBody(const PhysicsSystem::RigidBodyType& type, float x, float y)
{
    // sanity check -> valid body
    if (type == PhysicsSystem::RigidBodyType::kDynamic)
        m_bodyDef.type = b2_dynamicBody;
    else if (type == PhysicsSystem::RigidBodyType::kStatic)
        m_bodyDef.type = b2_staticBody;
    else
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutput, "Invalid Body");
    
    m_bodyDef.position.Set(x, y);
    m_bodyDef.fixedRotation = true;
    m_bodyDef.userData = m_pGameObject;
    m_pBody = PhysicsSystem::Get()->CreateRigidBody(&m_bodyDef);
}

void PhysicsComponent::CreateFixture(const PhysicsSystem::PolygonShapeType& type, float width, float height)
{
    // sanity check -> valid body
    if (m_pBody)
    {
        m_fixtureDef.shape = PhysicsSystem::Get()->CreatePolygon(type, width, height);
        m_fixtureDef.density = 1.f;
        m_pBody->CreateFixture(&m_fixtureDef);
    }
}
