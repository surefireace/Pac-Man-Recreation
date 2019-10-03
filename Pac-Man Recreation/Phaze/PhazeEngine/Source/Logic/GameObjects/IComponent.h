#pragma once
/// \file IComponent.h

// fwd declares
class GameObject;

/// \brief Interface for Components
class IComponent
{
private:
    /// \brief ID of current component
    unsigned int m_id;
protected:
    /// \brief GameObject attached to this component.
    GameObject* m_pGameObject;
public:
    // TODO: Move Type enums here

    /// \brief Attaches a game object to the component.
    /// \param pGameObject Pointer to game object to attach to.
    void AttachGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }

    /// \brief Detaches a game object to the component.
    void DetachGameObject() { m_pGameObject = nullptr; }

    /// \brief Returns the component ID
    unsigned int GetID() { return m_id; }

    /// \brief Sets the component ID
    void SetID(unsigned int newID) { m_id = newID; }

    /// Virtual destructor.
    virtual ~IComponent() = default;
};