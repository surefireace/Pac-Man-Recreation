#pragma once
// Created by Billy Graban

#include "IRenderable.h"
#include <string>
#include "../Utils/Macros.h"
#include <vector>
#include "../Logic/Events/EventSystem.h"

/// \file View.h

// fwd declares
struct SDL_Renderer;
struct SDL_Texture;
class GameObject;

/// \class View
/// \brief Handles display's and sounds of game
class View : public IRenderable, public IEventHandler
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Cleans up view
    void CleanUp();

    /// \brief Renders the game
    // TODO: Find a way to fix clients being able to call
    PHAZE_API virtual void Render() override;

    /// \brief Hanldes an event
    // TODO: Find a way to fix clients being able to call
    PHAZE_API virtual void HandleEvent(Event* pEvent) override;

private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// \brief SDL renderer to use
    SDL_Renderer* m_pRenderer;

    /// \brief Vector of Game objects to render.
    std::vector<GameObject*>* m_pGameObjectsToRender;

    /// \brief Vector of tiles to render
    std::vector<GameObject*>* m_pTilesToRender;

    /// \brief List of IRenderables to render.
    std::vector<IRenderable*> m_renderables;

    /// \brief A texture event.
    TextureEvent m_textureEvent;

    /// \brief Initialization flag
    bool m_isInitialized;

    /// \brief Reference to instance
    static View* m_pInstance;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Constructor.
    View();

    /// \brief Destructor.
    ~View();

public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// \brief Returns the initialization flag
    /// \return True or False if initialized
    const bool GetInitFlag() { return m_isInitialized; }

    /// \brief Adds a renderable to the list
    /// \param pRenderable IRenderable to add
    PHAZE_API void AddRenderable(IRenderable* pRenderable) { if (pRenderable) m_renderables.emplace_back(pRenderable); }

    /// \brief Gets the refernce to this instance.
    /// \return Pointer to this instance
    PHAZE_API static View* Get();

    /// \brief Destroys this instance
    static void Destroy();

    /// \brief Sets the renderer to use
    void SetRenderer(SDL_Renderer* pRenderer);
};