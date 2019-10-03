#pragma once
// Created by Michael Smith
/// \file MovementComponent.h
#include "IComponent.h"
#include "../Math/Vector2D.h"
#include "../IUpdatable.h"

// fwd declares
class GameObject;

/// \brief Component to hold and handle movement data
class MovementComponent : public IComponent, public IUpdatable
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //
    

    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// updates the movement component.
    virtual void Update(float deltaTime_sec) override;
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

};