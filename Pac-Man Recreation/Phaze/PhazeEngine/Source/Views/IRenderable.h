#pragma once

/// \file IRenderable.h
#include <SDL_rect.h>

// fwd declares
struct TextureComponent;

/// \class IRenderable
/// \brief Interface for anything renderable
class IRenderable
{
protected:
    /// \brief Horizontal flip flag
    bool m_horzFlip;

    /// \brief Diagonal flip flag
    bool m_diagFlip;

    /// \brief Vertical flip flag
    bool m_vertFlip;

    /// \brief Transform of renderable
    SDL_Rect m_transform;

    /// \brief Render flag
    bool m_isRendered;

    /// \brief TextureComponent attached to this renderable
    TextureComponent* m_pTextureComponent;
public:
    /// \brief Returns the sdl texture
    TextureComponent* GetTextureComponent() { if (m_pTextureComponent) return m_pTextureComponent; return nullptr; };

    /// \brief Returns the render flag
    /// \return The render flag
    const bool GetRenderFlag() { return m_isRendered; }

    /// \brief Sets the render flag
    /// \param flag The flag to set
    void SetRenderFlag(const bool flag) { m_isRendered = flag; }

    /// \brief Returns the transform
    const SDL_Rect GetTransform() { return m_transform; }

    /// \brief Sets the transform
    /// \param newRect SDL rect to be set
    void SetTransform(const SDL_Rect newRect) { m_transform = newRect; }

    /// \brief Retrives horizontal flip flag
    /// \return Returns the flip flag
    const bool IsHorzFlip() { return m_horzFlip; }

    /// \brief Sets horizontal flip flag
    /// \param flag What the flag will be set to.
    void SetHorzFlip(const bool flag) { m_horzFlip = flag; }

    /// \brief Retrives vertical flip flag
    /// \return Returns the flip flag
    const bool IsVertFlip() { return m_vertFlip; }

    /// \brief Sets vertical flip flag
    /// \param flag What the flag will be set to.
    void SetVertFlip(const bool flag) { m_vertFlip = flag; }

    /// \brief Retrives diagonal flip flag
    /// \return Returns the flip flag
    const bool IsDiagFlip() { return m_diagFlip; }

    /// \brief Sets diagonal flip flag
    /// \param flag What the flag will be set to.
    void SetDiagFlip(const bool flag) { m_diagFlip = flag; }

    /// \brief Interface function for a renderable
    virtual void Render() = 0;
};