#pragma once

/// \file PhysicsSystem.h

#include <Box2D.h>
#include <vector>
#include "../IUpdatable.h"
#include "../Events/EventSystem.h"

// fwd declares
struct SDL_Renderer;

/// \class PhysicsSystem
/// \brief Physics system for engine
class PhysicsSystem : public b2ContactListener, public IUpdatable, public b2Draw
{
private:
    /// \brief The current physics world
    b2World* m_pWorld;

    /// \brief Container of polygons
    std::vector<b2PolygonShape*> m_polygons;

    /// \brief Static instance of system
    static PhysicsSystem* m_pInstance;

    /// \brief Constructor.
    PhysicsSystem() {};

    /// \brief Start of collision event
    virtual void BeginContact(b2Contact* pContact) override;

    /// \brief End of collision event
    virtual void EndContact(b2Contact* pContact) override;

    /// \brief Allows altering of contacts BEFORE collision response is processed
    virtual void PreSolve(b2Contact*, const b2Manifold*) override;

    /// \brief Finds out the impulses caused by collision response AFTER collision application
    virtual void PostSolve(b2Contact*, const b2ContactImpulse*) override;

    /// \brief Renderer for debug drawing
    SDL_Renderer* m_pRenderer;
public:
    /// \brief Rigid Body types
    enum class RigidBodyType
    {
        kStatic,
        kDynamic,
        kNumOfTypes
    };

    /// \brief Polygon Shape types
    enum class PolygonShapeType
    {
        kBox,
        kNumOfTypes
    };
    /// \brief Creates a physics world. There can only be one physics world!
    /// \param gravity Gravity for the world
    void CreatePhysicsWorld(b2Vec2 gravity);

    /// \brief Creates a rigid body. This is needed for a world
    /// \param type Type of rigid body to create.
    /// \param xPos X position of rigid body
    /// \param yPos Y position of rigid body
    b2Body* CreateRigidBody(b2BodyDef* pBodyDef);

    /// \brief Updates the physics world
    virtual void Update(float deltaTime_sec) override;

    /// \brief Creates and returns a polygon shape
    /// \param type The type of polygon shape to make
    /// \param width Width of the shape to make
    /// \param height Height of the shape to make
    b2PolygonShape* CreatePolygon(const PolygonShapeType, float width, float height);

    /// \brief Cleans up the physics system from memory
    void Cleanup();

    /// \brief Returns an instance of the system
    static PhysicsSystem* Get();

    /// \brief Deletes the instance
    static void Delete();

    /// \brief Draws physics debug info
    void SetRenderer(SDL_Renderer* pRenderer) { m_pRenderer = pRenderer; }

    /// \brief Draws debug info
    void DrawDebugInfo() { if (m_pRenderer) m_pWorld->DrawDebugData(); }

    // Inherited via b2Draw
    virtual void DrawPolygon(const b2Vec2*, int32, const b2Color&) override;
    virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color&) override;
    virtual void DrawCircle(const b2Vec2&, float32, const b2Color&) override;
    virtual void DrawSolidCircle(const b2Vec2&, float32, const b2Vec2&, const b2Color&) override;
    virtual void DrawSegment(const b2Vec2&, const b2Vec2&, const b2Color&) override;
    virtual void DrawTransform(const b2Transform&) override;
    virtual void DrawPoint(const b2Vec2&, float32, const b2Color&) override;
};