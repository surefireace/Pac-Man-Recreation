#pragma once
// Created by Michael Smith

#include <map>
#include <SDL_mixer.h>
//#include "IResource.h"

/// \file ResourceSystem.h


// fwd declares
struct SDL_Texture;
struct SDL_Renderer;

/// \brief Interface for all engine resoureces
class IResource
{
public:
    /// \brief Resource types.
    enum class Type
    {
        kTexture,
        kSfx,
        kMusic,
        kNumOfTypes
    };
protected:
    /// \brief Resource ID.
    //unsigned long m_id;

    /// \brief Resource type
    Type m_type;

public:
    // accessors
    Type GetType() { return m_type; }
    //unsigned long GetID() { return m_id; }

    // mutators
    void SetType(const Type& type) { m_type = type; }

    // virtual dtor
    virtual ~IResource() {}
};

/// \brief Represents a texture resource
struct TextureResource : public IResource
{
    /// \brief Texture this resource represents
    SDL_Texture* m_pTexture;
};

/// \brief Represents a Sfx resource
struct SfxResource : public IResource
{
    /// \brief Sfx this resource represents
    Mix_Chunk* m_pSfx;
};

/// \brief Represents a Music resource
struct MusicResource : public IResource
{
    /// \brief Music this resource represents
    Mix_Music* m_pMusic;
};

/// \brief Overseer of all engine resources
class ResourceSystem
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
    ResourceSystem() = delete;

    /// \brief Loads a resource to resource map
    /// \param name Name for to be loaded resource
    /// \param filePath resource file to load
    /// \param type Type of resource to load
    static void LoadResource(const std::string& name, const std::string& filePath, const IResource::Type& type);

    /// \brief Cleans up the system
    static void CleanUp();

private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// \brief Renderer delegate
    static SDL_Renderer* m_pRenderer;

    /// \brief Map of loaded resources
    static std::map<std::string, IResource*> m_resourceMap;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //


public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //



    /// \brief Returns a resource from resource map
    /// \param name Name of resource to load
    static IResource* GetResource(const std::string& name) { return m_resourceMap[name] ? m_resourceMap[name] : nullptr; }

    /// \brief Sets the renderer to use
    static void SetRenderer(SDL_Renderer* pRenderer) { if (pRenderer) m_pRenderer = pRenderer; }
};