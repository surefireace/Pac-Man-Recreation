#pragma once

/// \file MapSystem.h
#include <vector>
#include "../Utils/tinyxml2.h"
#include <SDL_rect.h>
#include <unordered_map>
#include "../Logic/Events/EventSystem.h"

// fwd declares
struct SDL_Texture;
class ObjectPool;
class ComponentPool;

/// \class MapSystem
/// \brief Handles all tile maps
class MapSystem : public IEventHandler
{
private:
    /// \brief texture event
    TextureEvent m_textureEvent;

    /// \brief game object pool
    ObjectPool* m_pObjPool;

    /// \brief component pool
    ComponentPool* m_pComponentPool;

    /// \brief map event
    MapEvent m_mapEvent;

    /// \brief tiles to send off to event system for rendering
    std::vector<GameObject*> m_tilesToRender;

    /// \brief tile set structure
    struct TileSet
    {
        /// \brief tile set name
        std::string m_name;

        /// \brief num of tlies
        //int m_tileCount;

        /// \brief num of columns
        int m_numOfColumns;

        /// \brief index of first gid
        int m_firstGid;

        /// \brief tiles width
        int m_tileWidth;

        /// \brief tile height
        int m_tileHeight;

        /// \brief source image
        SDL_Texture* m_pSrcTexture;
    };

    /// \brief Collider structure
    struct Collider
    {
        int m_id;
        std::string m_type;
        int m_xPos;
        int m_yPos;
        int m_width; 
        int m_height;
        SDL_Rect m_destRect;
    };

    /// \brief tile structure
    struct Tile
    {
        /// \brief tile id
        int m_gid;

        /// \brief tile gid releative to tileset
        int m_relativeGid;

        /// \brief source rect
        SDL_Rect m_srcRect;

        /// \brief destination rect
        SDL_Rect m_destRect;

        /// \brief tileset this tile belongs to
        TileSet m_tileSetOwner;

        /// \brief vertical flip flag
        bool m_vertFlip;

        /// \brief horizontal flip flag
        bool m_horzFlip;

        /// \brief diagonal flip flag
        bool m_diagFlip;

    };

    /// \brief layer data structure
    struct Layer
    {
        /// \brief layer width
        int m_width;

        /// \brief layer height
        int m_height;

        /// \brief name of layer
        std::string m_name;

        /// \brief collection of tiles
        std::vector<Tile> m_tiles;
    };

    /// \struct map data
    struct TileMap
    {
        /// \brief map width
        int m_width;

        /// \brief map height
        int m_height;

        /// \brief tile width
        int m_tileWidth;

        /// \brief tile height
        int m_tileHeight;

        /// \brief tile set container
        std::vector<TileSet> m_tileSets;

        /// \brief map layer container
        std::vector<Layer> m_layers;

        /// \brief colldier container
        std::vector<Collider> m_colliders;

        /// \brief retrives a tileset
        TileSet GetTileSet(int gid);

    };

    /// \brief Constructor
    MapSystem();

    /// \brief Reference to instance
    static MapSystem* m_pInstance;

    /// \brief Current map index
    int m_currentMap;

    /// \brief Container of maps
    std::vector<TileMap> m_maps;
public:
    /// \brief Loads a map from file
    bool LoadMap(const std::string& tmxFile);
    
    /// \brief Cleans map system
    void CleanUp();

    /// \brief Returns the instance
    static MapSystem* Get();

    /// \brief Deletes the instance
    static void Delete();

    /// \brief Inherited via IEventHandler
    virtual void HandleEvent(Event* pEvent) override;

    /// \brief Sets the game object pool to use
    void SetObjPool(ObjectPool* pObjPool) { m_pObjPool = pObjPool; }

    /// \brief Sets the component pool to use
    void SetComponentPool(ComponentPool* pCompPool) { m_pComponentPool = pCompPool; }
};