#pragma once
// Created by Michael Smith

/// \file ComponentPool.h

#include <memory>
#include <unordered_map>

// fwd declares
class IComponent;

// typedefs
typedef std::shared_ptr<IComponent> ComponentPtr;

/// \brief Pool of IComponents
class ComponentPool
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //

    // TODO: make configurable and/or data driven or find another way besides enums
    /// Component types.
    enum class Type
    {
        kMove,
        kTexture,
        kPhysics,
        kNumOfCompnentTypes
    };

    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Creates an IComponent and returns a pointer to it.
    /// \param type Type of component to be created.
    IComponent* CreateComponent(const Type& type);

    /// \brief Deletes an IComponent from the pool.
    /// \param componentID ID of the component to delete.
    void DeleteComponent(unsigned int componentID);

    /// Initializes the pool.
    bool Init();

private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// Pool of IComponents.
    std::unordered_multimap<unsigned int, ComponentPtr> m_components;

    /// Amount of components allowed.
    static const int s_kComponentLimit;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //


public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //
    

};