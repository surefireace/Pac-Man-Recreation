#include "ResourceSystem.h"
#include "../Utils/PhazeLogger.h"
#include <SDL_image.h>

// consts
const std::string kLogOutputFile = "Logs/ResourceSystemLog.txt";

// statics
SDL_Renderer* ResourceSystem::m_pRenderer;
std::map<std::string, IResource*> ResourceSystem::m_resourceMap;

void ResourceSystem::LoadResource(const std::string& name, const std::string& filePath, const IResource::Type& type)
{
    // capture cstring
    const char* pName = name.c_str();
    
    // load resources based on type
    switch (type)
    {
        case IResource::Type::kTexture:
        {
            // sanity check -> texture already loaded
            auto iter = m_resourceMap.find(pName);
            if (m_resourceMap.end() != iter)
                PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Texture: %s already loaded!", pName);

            // sanity check -> surface creation
            SDL_Surface* pSurface = IMG_Load(filePath.c_str());
            if (!pSurface)
                PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Error creating sdl surface for texture: %s %s", pName, IMG_GetError());

            // sanity check -> texture creation
            SDL_Texture* pTexture = SDL_CreateTextureFromSurface(m_pRenderer, pSurface);
            SDL_FreeSurface(pSurface);
            if (!pTexture)
                PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Error creating sdl texture for texture: %s %s", pName, IMG_GetError());

            // TODO: Pool resources
            // create resource and set its attributes
            TextureResource* pResource = new TextureResource;
            pResource->m_pTexture = pTexture;
            pResource->SetType(IResource::Type::kTexture);
            m_resourceMap[pName] = pResource;

            // log it
            PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Texture: %s loaded!", pName);
            break;
        }

        case IResource::Type::kSfx:
        {
            // map iterator
            auto iter = m_resourceMap.find(pName);

            // sanity check -> sfx loaded already
            if (m_resourceMap.end() == iter)
            {
                // load the sfx from the file
                // TODO: Pool resources
                // create resource and set its attributes
                SfxResource* pResource = new SfxResource;
                pResource->m_pSfx = Mix_LoadWAV(filePath.c_str());
                pResource->SetType(IResource::Type::kSfx);
                m_resourceMap[pName] = pResource;

                if (!m_resourceMap[pName])
                {
                    PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, Mix_GetError());
                }

                PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Sound: %s loaded!", pName);
            }
            else
            {
                PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Sound: %s already loaded!", pName);
            }
            
            break;
        }

        case IResource::Type::kMusic:
        {
            // map iterator
            auto iter = m_resourceMap.find(pName);

            // sanity check -> music loaded already
            if (m_resourceMap.end() == iter)
            {
                // load the music from the file
                // TODO: Pool resources
                // create resource and set its attributes
                MusicResource* pResource = new MusicResource;
                pResource->m_pMusic = Mix_LoadMUS(filePath.c_str());
                pResource->SetType(IResource::Type::kMusic);
                m_resourceMap[pName] = pResource;

                if (!m_resourceMap[pName])
                {
                    PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, Mix_GetError());
                }

                PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Music: %s loaded!", pName);
            }
            else
            {
                PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Music: %s already loaded!");
            }
            
            break;
        }
        
        default:
        {
            break;
        }
    }
}

void ResourceSystem::CleanUp()
{
    // iterate through each resource
    for (std::pair<std::string, IResource*> pair : m_resourceMap)
    {
        // different types need to be deleted differently
        switch (pair.second->GetType())
        {
            case IResource::Type::kTexture:
            {
                TextureResource* pTextureResource = static_cast<TextureResource*>(pair.second);
                SDL_DestroyTexture(pTextureResource->m_pTexture);
                delete pTextureResource;

                break;
            }

            case IResource::Type::kSfx:
            {
                SfxResource* pSfxResource = static_cast<SfxResource*>(pair.second);
                Mix_FreeChunk(pSfxResource->m_pSfx);
                delete pSfxResource;

                break;
            }

            case IResource::Type::kMusic:
            {
                MusicResource* pMusicResource = static_cast<MusicResource*>(pair.second);
                Mix_FreeMusic(pMusicResource->m_pMusic);
                delete pMusicResource;
                
                break;
            }

            default:
            {
                break;
            }
        }
    }
    m_resourceMap.clear();
}
