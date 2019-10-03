#pragma once
// Created by Michael Smith

#include <map>
#include <vector>
#include <SDL_rect.h>
#include "../../Utils/Macros.h"

/// \file EventSystem.h

// fwd declares
class GameObject;
struct SDL_Texture;

/// \brief Event structure
struct Event
{
    enum class Type
    {
        kGame,
        kMap,
        kView,
        kLoadSfx,
        kPlaySfx,
        kLoadMusic,
        kPlayMusic,
        kStopMusic,
        kLoadTexture,
        kRequestTexture,
        kRequestMapTexture,
        kSendTexture,
        kSendMapTexture,
        kNumOfTypes
    };

    /// \brief Type of event
    Type m_type;

    /// \brief TEST: only here to test sending and recieving of events
    std::string m_eventStr;
};

/// \brief Structure for Game Logic Events
struct GameEvent : public Event
{
    /// \brief List of this event's game objects
    std::vector<GameObject*>* m_pGameObjects;
};

/// \brief Structure for Map System Events
struct MapEvent : public Event
{
    /// \brief List of this event's tiles
    std::vector<GameObject*>* m_pTiles;
};

/// \brief Structure for Sfx Events
struct SfxEvent : public Event
{
    /// \brief Name of the sfx
    std::string m_sfxName;

    /// \brief Filename for the sfx
    std::string m_sfxFile;
};

/// \brief Structure for Music Events
struct MusicEvent : public Event
{
    /// \brief Name of the music
    std::string m_musicName;

    /// \brief Filename for the music
    std::string m_musicFile;
};

/// \brief Structure for Texture events
struct TextureEvent : public Event
{
    /// \brief Name of the texture
    std::string m_textureName;

    /// \brief Filename for the texture
    std::string m_textureFile;

    /// \brief The texture data
    SDL_Texture* m_pTexture;

    /// \brief The game object that needs the texture
    GameObject* m_pGameObject;
};

/// \brief Interface for Event handlers
class IEventHandler
{
public:
    /// \brief Handles the event
    /// \param event Event to handle
    virtual void HandleEvent(Event* pEvent) = 0;
};

// typedefs
typedef std::vector<IEventHandler*> EventHandlers;

/// \brief System to handle sending and recieving of Events
class EventSystem
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// Constructor
    /// Not ment to be invoked
    EventSystem() = delete;


    /// \brief Subscribes a handler to an event
    /// \param eventType The type of event to subscribe the handler to
    /// \param pHandler pointer to the handler that will be subscribed
    static void SubscribeHandler(const Event::Type& eventType, IEventHandler* pHandler);

    /// \brief Unsubscribes a handler from an event
    /// \param eventType The type of event to unsubscribe the handler from
    /// \param pHandler pointer to the handler that will be unsubscribed
    static void UnsubscribeHandler(const Event::Type& eventType, IEventHandler* pHandler);

    /// \brief Broadcasts an event to all its handlers
    /// \param pEvent pointer to Event to broadcast
    PHAZE_API static void BroadcastEvent(Event* pEvent);


private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //
    /// \brief Table of IEventHandlers mapped to a single Event::Type
    static std::map<Event::Type, EventHandlers> m_HandlerMap;



    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //


public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //


};