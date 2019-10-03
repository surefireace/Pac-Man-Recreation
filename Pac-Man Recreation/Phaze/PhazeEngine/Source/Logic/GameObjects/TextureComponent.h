#pragma once
// Created by Michael Smith

/// \file TextureComponent.h
#include "IComponent.h"
#include <string>
#include <SDL_rect.h>

// fwd declares
struct SDL_Texture;

/// \brief Structure to store texture data
struct TextureComponent : public IComponent
{
    /// \brief Texture name
    // HACK: think of a systmatic way to store component data names
    std::string m_name;

    /// \brief Texture data
    SDL_Texture* m_pTexture;

    /// \brief Source rect for textures
    SDL_Rect m_srcRect;
};