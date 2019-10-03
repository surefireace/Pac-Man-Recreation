#pragma once
// Created by Michael Smith


#include <string>
#include <SDL_rect.h>
#include <map>
#include "../../Views/IRenderable.h"
#include "../../Utils/Macros.h"

// fwd declares
struct SDL_Texture;
struct SDL_Renderer;

// type defs
typedef struct _TTF_Font TTF_Font;

/// \struct TextField
/// \brief Represents a text field
struct TextField
{
    std::string m_text; ///> Text inside the field
    SDL_Texture* m_pTexture; ///> Texture itself
    SDL_Rect m_destRect; ///> Destination rect
    SDL_Color m_textColor; ///> Text color
    SDL_Color m_fieldColor; ///> Color of text field
};


/// \class UISystem
/// \brief System that handles all UI elements
class UISystem
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

protected:
    // --------------------------------------------------------------------- //
    // Protected Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Protected Member Functions
    // --------------------------------------------------------------------- //




private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// \brief Pointer to current font
    TTF_Font* m_pCurrFont;

    /// \brief table of text fields.
    std::map<const std::string, TextField> m_textFields;

    /// \brief table of loaded fonts.
    std::map<std::string, TTF_Font*> m_fonts;

    /// \brief reference to sdl renderer.
    SDL_Renderer* m_pRenderer;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Constructor.
    UISystem() {};

    /// \brief Destructor.
    ~UISystem();

    /// \brief instance of UI system.
    static UISystem* m_pInstance;

public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// \brief Returns an instance of UI system.
    /// \return Returns m_pInstance.
    PHAZE_API static UISystem* Get();

    /// \brief Destroys the instance of UI system.
    static void Destroy();

    /// \brief Clears all ui elements.
    PHAZE_API void ClearUI();

    /// \brief Loads a font into system.
    /// \param name The name of the font. (Can be whatever you want)
    /// \param filePath The filepath of the font to load
    /// \param size The font size.
    PHAZE_API bool LoadFont(const std::string& name, const std::string& filePath, const int size);

    /// \brief Selects the current font
    PHAZE_API bool SelectFont(const std::string& name);

    /// \brief Renders the UI System
    /// \param pRenderer The renderer to use.
    void Render(SDL_Renderer* pRenderer);

    /// \brief Adds a text field to m_textFields.
    /// \param name Name of the textfield.
    /// \param textField The textField to add.
    PHAZE_API void AddTextField(const std::string& name, const TextField& textField);
};