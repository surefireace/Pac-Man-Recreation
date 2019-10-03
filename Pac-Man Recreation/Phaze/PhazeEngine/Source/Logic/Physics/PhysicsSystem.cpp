#include "PhysicsSystem.h"
#include "../GameObjects/GameObject.h" //< TODO: find a better way to handle game object collision
#include "../../Utils/PhazeLogger.h"
#include <SDL_render.h>

// TODO: make this configurable
const std::string kLogOutputFile = "Logs/PhysicsLog.txt";

// statics
PhysicsSystem* PhysicsSystem::m_pInstance = nullptr;

void PhysicsSystem::BeginContact(b2Contact* pContact)
{
    GameObject* pObjA = static_cast<GameObject*>(pContact->GetFixtureA()->GetBody()->GetUserData());
    GameObject* pObjB = static_cast<GameObject*>(pContact->GetFixtureB()->GetBody()->GetUserData());

    // log the collision
    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "GameObject: %s collided with GameObject: %s", pObjA->GetObjName().c_str(), pObjB->GetObjName().c_str());
}

void PhysicsSystem::EndContact(b2Contact* pContact)
{
    GameObject* pObjA = static_cast<GameObject*>(pContact->GetFixtureA()->GetBody()->GetUserData());
    GameObject* pObjB = static_cast<GameObject*>(pContact->GetFixtureB()->GetBody()->GetUserData());

    // log the collision
    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "GameObject: %s has ended contact with GameObject: %s", pObjA->GetObjName().c_str(), pObjB->GetObjName().c_str());
}

void PhysicsSystem::PreSolve(b2Contact *, const b2Manifold *)
{
}

void PhysicsSystem::PostSolve(b2Contact *, const b2ContactImpulse *)
{
}

void PhysicsSystem::CreatePhysicsWorld(b2Vec2 gravity)
{
    // load world
    m_pWorld = new b2World(gravity);
    m_pWorld->SetContactListener(this);
    m_pWorld->SetDebugDraw(this);
    SetFlags(b2Draw::e_shapeBit); // sets the draw flag to draw shapes
}

b2Body* PhysicsSystem::CreateRigidBody(b2BodyDef* pBodyDef)
{
    return m_pWorld->CreateBody(pBodyDef);
}

void PhysicsSystem::Update(float deltaTime_sec)
{
    // iteration counters
    int velocityIterations = 6;
    int positionIterations = 2;

    // step world
    m_pWorld->Step(deltaTime_sec, velocityIterations, positionIterations);
}

b2PolygonShape* PhysicsSystem::CreatePolygon(const PolygonShapeType, float width, float height)
{
    b2PolygonShape* pPolygon = new b2PolygonShape();

    float halfWidth = width * 0.5f;
    float halfHeight = height * 0.5f;

    b2Vec2 center;
    center.x = halfWidth;
    center.y = halfHeight;

    pPolygon->SetAsBox(halfWidth, halfHeight, center, 0);

    m_polygons.emplace_back(pPolygon);

    return pPolygon;
}

void PhysicsSystem::Cleanup()
{
    for (b2PolygonShape* pPolygon : m_polygons)
        delete pPolygon;
    m_polygons.clear();

    // delete world
    delete m_pWorld;
}

PhysicsSystem* PhysicsSystem::Get()
{
    // sanity check -> valid instance
    if (!m_pInstance)
    {
        m_pInstance = new PhysicsSystem();
        return m_pInstance;
    }

    return m_pInstance;
}

void PhysicsSystem::Delete()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

void PhysicsSystem::DrawPolygon(const b2Vec2 *, int32, const b2Color &)
{
}

void PhysicsSystem::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color&)
{
    // sanity check -> valid renderer
    if (m_pRenderer)
    {
        // draw each point as a red point
        SDL_SetRenderDrawColor(m_pRenderer, 255, 0, 0, 0);
        for (int32 i = 0; i < vertexCount; ++i)
        {
            // sanity check -> last vert
            if (i + 1 == vertexCount)
            {
                SDL_RenderDrawLine(m_pRenderer, int(vertices[0].x * kMetersPerPixel), int(vertices[0].y * kMetersPerPixel),
                    int(vertices[i].x * kMetersPerPixel), int(vertices[i].y * kMetersPerPixel));
            }
            else
            {

                SDL_RenderDrawLine(m_pRenderer, int(vertices[i].x * kMetersPerPixel), int(vertices[i].y * kMetersPerPixel),
                    int(vertices[i + 1].x * kMetersPerPixel), int(vertices[i + 1].y * kMetersPerPixel));
            }
        }
    }
}

void PhysicsSystem::DrawCircle(const b2Vec2 &, float32, const b2Color &)
{
}

void PhysicsSystem::DrawSolidCircle(const b2Vec2 &, float32, const b2Vec2 &, const b2Color &)
{
}

void PhysicsSystem::DrawSegment(const b2Vec2 &, const b2Vec2 &, const b2Color &)
{
}

void PhysicsSystem::DrawTransform(const b2Transform &)
{
}

void PhysicsSystem::DrawPoint(const b2Vec2 &, float32, const b2Color &)
{
}
