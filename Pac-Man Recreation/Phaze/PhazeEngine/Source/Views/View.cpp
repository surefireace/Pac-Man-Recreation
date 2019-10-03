#include "View.h"
#include <SDL_render.h>
#include "../Logic/GameObjects/GameObject.h"
#include "../Utils/PhazeLogger.h"
#include "../Logic/GameObjects/TextureComponent.h"
#include "ResourceSystem.h"
#include "MapSystem.h"
#include "../Logic/Physics/PhysicsSystem.h"
#include "../Logic/UI/UISystem.h"

// TODO: Make this configurable
const std::string kLogOutputFile = "Logs/ViewLog.txt";

// static defines
View* View::m_pInstance = nullptr;

void View::CleanUp()
{
    ResourceSystem::CleanUp();
}

void View::Render()
{
    SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 0);
    SDL_RenderClear(m_pRenderer);

    for (IRenderable* pRenderable : m_renderables)
    {
        // check if we need to render
        if (pRenderable->GetRenderFlag())
        {
            pRenderable->Render();
            TextureComponent* pTextureComponent = pRenderable->GetTextureComponent();
            SDL_Rect* pSrcRect = &pTextureComponent->m_srcRect;
            const SDL_Rect objRect = pRenderable->GetTransform();

            // sanity check -> is there a src rect?
            if (pSrcRect->x == -1)
                pSrcRect = nullptr;

            // HACK: See if there is a better way to handle rotations and flips of tiles.
                // all directions are as if rotating clockwise
                // 90 degrees
            if (pRenderable->IsDiagFlip() && pRenderable->IsHorzFlip())
            {
                SDL_RenderCopyEx(m_pRenderer, pTextureComponent->m_pTexture, pSrcRect, &objRect, 90.f, nullptr, SDL_FLIP_NONE);
            }
            // 180 degrees
            else if (pRenderable->IsVertFlip() && pRenderable->IsHorzFlip())
            {
                SDL_RenderCopyEx(m_pRenderer, pTextureComponent->m_pTexture, pSrcRect, &objRect, 180.f, nullptr, SDL_FLIP_NONE);
            }
            // 270 degrees
            else if (pRenderable->IsVertFlip() && pRenderable->IsDiagFlip())
            {
                SDL_RenderCopyEx(m_pRenderer, pTextureComponent->m_pTexture, pSrcRect, &objRect, 270.f, nullptr, SDL_FLIP_NONE);
            }
            else
            {
                SDL_RenderCopy(m_pRenderer, pTextureComponent->m_pTexture, pSrcRect, &objRect);
            }
        }
    }
    UISystem::Get()->Render(m_pRenderer);
    PhysicsSystem::Get()->DrawDebugInfo();
    SDL_RenderPresent(m_pRenderer);

}

PHAZE_API void View::HandleEvent(Event* pEvent)
{
    // handle the event based on event type
    switch (pEvent->m_type)
    {
        case Event::Type::kGame:
        {
            // static cast
            GameEvent* pGameEvent = static_cast<GameEvent*>(pEvent);

            // sanity check -> retrival of game objects to render
            if (pGameEvent->m_pGameObjects)
                m_pGameObjectsToRender = pGameEvent->m_pGameObjects;
            
            break;
        }

        case Event::Type::kMap:
        {
            // static cast
            MapEvent* pMapEvent = static_cast<MapEvent*>(pEvent);

            // sanity check -> retrival of tiles to render
            if (pMapEvent->m_pTiles)
                m_pTilesToRender = pMapEvent->m_pTiles;

            break;
        }

        case Event::Type::kLoadSfx:
        {
            // static cast
            SfxEvent* pSfxEvent = static_cast<SfxEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pSfxEvent->m_sfxFile != "" && pSfxEvent->m_sfxName != "")
            {
                //PHAZE_LOAD_SFX(pSfxEvent->m_sfxName, pSfxEvent->m_sfxFile);

                // TEST: resource system load
                ResourceSystem::LoadResource(pSfxEvent->m_sfxName, pSfxEvent->m_sfxFile, IResource::Type::kSfx);
            }

            break;
        }

        case Event::Type::kPlaySfx:
        {
            // static cast
            SfxEvent* pSfxEvent = static_cast<SfxEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pSfxEvent->m_sfxName != "")
            {
                //PHAZE_PLAY_SFX(pSfxEvent->m_sfxName);

                // TEST: resource system retrival
                SfxResource* pSfx = static_cast<SfxResource*>(ResourceSystem::GetResource(pSfxEvent->m_sfxName));

                // play the sound
                int channel = Mix_PlayChannel(1, pSfx->m_pSfx, 0);

                if (channel == -1)
                    PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, Mix_GetError());
            }

            break;
        }

        case Event::Type::kLoadMusic:
        {
            // static cast
            MusicEvent* pMusicEvent = static_cast<MusicEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pMusicEvent->m_musicName != "" && pMusicEvent->m_musicFile != "")
            {
                //PHAZE_LOAD_MUSIC(pMusicEvent->m_musicName, pMusicEvent->m_musicFile);

                ResourceSystem::LoadResource(pMusicEvent->m_musicName, pMusicEvent->m_musicFile, IResource::Type::kMusic);
            }

            break;
        }

        case Event::Type::kPlayMusic:
        {
            // static cast
            MusicEvent* pMusicEvent = static_cast<MusicEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pMusicEvent->m_musicName != "")
            {
                //PHAZE_PLAY_MUSIC(pMusicEvent->m_musicName);

                MusicResource* pMusic = static_cast<MusicResource*>(ResourceSystem::GetResource(pMusicEvent->m_musicName));

                // play the sound
                if (Mix_PlayingMusic())
                    Mix_HaltMusic();

                if (Mix_PlayMusic(pMusic->m_pMusic, -1) == -1)
                    PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, Mix_GetError());
            }

            break;
        }

        case Event::Type::kStopMusic:
        {
            if (Mix_PlayingMusic())
                Mix_HaltMusic();

            break;
        }

        case Event::Type::kLoadTexture:
        {
            // static cast
            TextureEvent* pTextureEvent = static_cast<TextureEvent*>(pEvent);

            // sanity check -> retrival of texture data
            if (pTextureEvent->m_textureName != "" && pTextureEvent->m_textureFile != "")
            {
                //PHAZE_LOAD_TEXTURE(pTextureEvent->m_textureName, pTextureEvent->m_textureFile, m_pRenderer);

                // load texture
                ResourceSystem::LoadResource(pTextureEvent->m_textureName, pTextureEvent->m_textureFile, IResource::Type::kTexture);
            }

            break;
        }

        case Event::Type::kRequestTexture:
        {
            // static cast
            TextureEvent* pTextureEvent = static_cast<TextureEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pTextureEvent->m_textureName != "")
            {
                // grab texture to send
                //m_textureEvent.m_pTexture = PHAZE_GET_TEXTURE(pTextureEvent->m_textureName);
                //m_textureEvent.m_type = Event::Type::kSendTexture;

                // grab our texture from the system then setup the event
                TextureResource* pTextureResource = static_cast<TextureResource*>(ResourceSystem::GetResource(pTextureEvent->m_textureName));
                pTextureEvent->m_pGameObject->GetTextureComponent()->m_pTexture = pTextureResource->m_pTexture;
                //m_textureEvent.m_textureName = pTextureEvent->m_textureName;
                //m_textureEvent.m_pTexture = pTextureResource->m_pTexture;
                //m_textureEvent.m_pGameObject->GetTextureComponent()->m_pTexture = pTextureResource->m_pTexture;
                //m_textureEvent.m_type = Event::Type::kSendTexture;

                // send texture
                //EventSystem::BroadcastEvent(&m_textureEvent);
            }

            break;
        }

        case Event::Type::kRequestMapTexture:
        {
            // static cast
            TextureEvent* pTextureEvent = static_cast<TextureEvent*>(pEvent);

            // sanity check -> retrival of sfx data
            if (pTextureEvent->m_textureName != "")
            {
                // grab texture to send
                //m_textureEvent.m_pTexture = PHAZE_GET_TEXTURE(pTextureEvent->m_textureName);
                //m_textureEvent.m_type = Event::Type::kSendTexture;

                // grab our texture from the system then setup the event
                TextureResource* pTextureResource = static_cast<TextureResource*>(ResourceSystem::GetResource(pTextureEvent->m_textureName));
                m_textureEvent.m_textureName = pTextureEvent->m_textureName;
                m_textureEvent.m_pTexture = pTextureResource->m_pTexture;
                m_textureEvent.m_type = Event::Type::kSendMapTexture;

                // send texture
                EventSystem::BroadcastEvent(&m_textureEvent);
            }

            break;
        }

        default:
        {
            break;
        }
    }
}

View::View()
{
    // subscribe to events
    EventSystem::SubscribeHandler(Event::Type::kGame, this);
    EventSystem::SubscribeHandler(Event::Type::kMap, this);
    EventSystem::SubscribeHandler(Event::Type::kLoadSfx, this);
    EventSystem::SubscribeHandler(Event::Type::kPlaySfx, this);
    EventSystem::SubscribeHandler(Event::Type::kLoadMusic, this);
    EventSystem::SubscribeHandler(Event::Type::kPlayMusic, this);
    EventSystem::SubscribeHandler(Event::Type::kStopMusic, this);
    EventSystem::SubscribeHandler(Event::Type::kLoadTexture, this);
    EventSystem::SubscribeHandler(Event::Type::kRequestTexture, this);
    EventSystem::SubscribeHandler(Event::Type::kRequestMapTexture, this);

    // set init flag
    m_isInitialized = true;
}

View::~View()
{
    ResourceSystem::CleanUp();
    UISystem::Get()->Destroy();
}

View* View::Get()
{
    if (!m_pInstance)
    {
        m_pInstance = new View();
        PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "View allocated!");
    }
    
    return m_pInstance;
}

void View::Destroy()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

void View::SetRenderer(SDL_Renderer* pRenderer)
{
    if (pRenderer)
    {
        m_pRenderer = pRenderer;
        PhysicsSystem::Get()->SetRenderer(m_pRenderer);
        ResourceSystem::SetRenderer(m_pRenderer);
    }
}
