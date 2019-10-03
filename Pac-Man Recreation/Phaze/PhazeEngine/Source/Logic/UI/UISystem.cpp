#include "UISystem.h"
#include "SDL_ttf.h"
#include <SDL_render.h>
#include "../../Utils/PhazeLogger.h"

// consts
constexpr int kTextFieldSize = 128;
const std::string kLogOutputFile = "Logs/UILog.txt";

// static(s)
UISystem* UISystem::m_pInstance = nullptr;

UISystem::~UISystem()
{
    // clean ui
    ClearUI();

    // close fonts
    for (std::pair<std::string, TTF_Font*> pair : m_fonts)
        TTF_CloseFont(pair.second);
    m_pCurrFont = nullptr;
}

UISystem* UISystem::Get()
{
    if (!m_pInstance)
        m_pInstance = new UISystem();
    
    return m_pInstance;
}

void UISystem::Destroy()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void UISystem::ClearUI()
{
    // clear text fields
    //for (std::pair<std::string, TextField> pair : m_textFields)
    //    SDL_DestroyTexture(pair.second.m_pTexture);
    m_textFields.clear();
}

bool UISystem::LoadFont(const std::string& name, const std::string& filePath, const int size)
{
    TTF_Font* pFont = TTF_OpenFont(filePath.c_str(), size);
    if (pFont)
    {
        m_fonts[name] = pFont;
        return true;
    }
    return false;
}

bool UISystem::SelectFont(const std::string& name)
{
    auto it = m_fonts.find(name);
    if (it != m_fonts.end())
    {
        m_pCurrFont = m_fonts[name];
        return true;
    }
    return false;
}

void UISystem::Render(SDL_Renderer* pRenderer)
{
    // sanity check -> valid renderer
    if (!pRenderer)
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Invalid SDL Renderer!\n");
    // sanity check -> valid font
    if (!m_pCurrFont)
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Trying render an Invalid font! Maybe you need to use SelectFont() first?\n");

    for (std::pair<std::string, TextField> pair : m_textFields)
    {
        TextField textField(pair.second);
        
        // setup sdl texture and surface
        SDL_Surface* pTxtFieldSurface = TTF_RenderUTF8_Solid(m_pCurrFont, textField.m_text.c_str(), textField.m_textColor); // Create a surface
        SDL_Texture* pTxtFieldTexture = SDL_CreateTextureFromSurface(pRenderer, pTxtFieldSurface); // Create a texture

        // setup textfield properties
        textField.m_pTexture = pTxtFieldTexture;
        textField.m_destRect.w = pTxtFieldSurface->w;
        textField.m_destRect.h = pTxtFieldSurface->h;
        SDL_FreeSurface(pTxtFieldSurface); // Always remember to free the surface

        // send to renderer
        SDL_SetRenderDrawColor(pRenderer, textField.m_fieldColor.r, textField.m_fieldColor.g, textField.m_fieldColor.b, textField.m_fieldColor.a);
        SDL_RenderFillRect(pRenderer, &textField.m_destRect);
        SDL_RenderCopy(pRenderer, textField.m_pTexture, nullptr, &textField.m_destRect);
    }
}

void UISystem::AddTextField(const std::string& name, const TextField& textField)
{
    m_textFields[name] = textField;
}
