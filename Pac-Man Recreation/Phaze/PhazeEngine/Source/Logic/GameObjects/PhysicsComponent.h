#pragma once

/// \file PhysicsComponent.h
#include "IComponent.h"
#include "../Physics/PhysicsSystem.h"

// fwd declares

/// \class PhysicsComponent
/// \brief Component that enables physics
struct PhysicsComponent : public IComponent
{
    /// \brief Body of the game object
    b2Body* m_pBody;

    /// \brief Fixture Definition
    b2FixtureDef m_fixtureDef;

    /// \brief Body Definition
    b2BodyDef m_bodyDef;

    /// \brief Polygon for body
    b2PolygonShape* m_pBodyPolygon;

    void CreateBody(const PhysicsSystem::RigidBodyType& type, float x, float y);

    /// \brief Creates a fixture for the attached body
    void CreateFixture(const PhysicsSystem::PolygonShapeType& type, float width, float height);
};