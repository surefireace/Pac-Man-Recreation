#include "MapSystem.h"
#include "../Utils/PhazeLogger.h"
#include <sstream>
#include "../Logic/GameObjects/ObjectPool.h"
#include "../Logic/GameObjects/GameObject.h"
#include "../Logic/GameObjects/TextureComponent.h"
#include "../Logic/GameObjects/PhysicsComponent.h"
#include "View.h"

// static(s)
MapSystem* MapSystem::m_pInstance = nullptr;

const char* kLogFile = "Logs/MapLog.txt";

MapSystem::MapSystem()
    : m_currentMap(-1)
{
    // sub to events
    EventSystem::SubscribeHandler(Event::Type::kSendMapTexture, this);
}

/* 
    From tiled docs:
    The highest three bits of the gid store the flipped states. Bit 32 is used for storing whether the tile is horizontally
    flipped, bit 31 is used for the vertically flipped tiles and bit 30 indicates whether the tile is flipped (anti) diagonally,
    enabling tile rotation. These bits have to be read and cleared before you can find out which tileset a tile belongs to.
*/
constexpr unsigned kHorzFlipFlag = 0x80000000;
constexpr unsigned kVertFlipFlag = 0x40000000;
constexpr unsigned kDiagFlipFlag = 0x20000000;

bool MapSystem::LoadMap(const std::string& tmxFile)
{
    // sanity check -> valid file
    tinyxml2::XMLDocument mapDoc;
    if (mapDoc.LoadFile(tmxFile.c_str()) != 0)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogFile, "Unable to load file: %s", tmxFile.c_str());
        return false;
    }

    //----------------------------------------------------------------------------------------------------
    // Parsing tile map from xml
    //----------------------------------------------------------------------------------------------------
    ++m_currentMap; // increment current map
    tinyxml2::XMLElement* pMapRoot = mapDoc.RootElement(); // grab root
    TileMap map; // tile map instance
    map.m_width = pMapRoot->IntAttribute("width");
    map.m_height = pMapRoot->IntAttribute("height");
    map.m_tileWidth = pMapRoot->IntAttribute("tilewidth");
    map.m_tileHeight = pMapRoot->IntAttribute("tileheight");

    // emplace map
    m_maps.emplace_back(map);

    //----------------------------------------------------------------------------------------------------
    // Parsing tile set(s) from xml
    //----------------------------------------------------------------------------------------------------
    tinyxml2::XMLElement* pTileSetElement = pMapRoot->FirstChildElement();
    
    while (pTileSetElement)
    {
        // splice for correct path
        std::string tileSetPath = pTileSetElement->Attribute("source");
        std::size_t dirEndPos = tmxFile.find_last_of('\\');
        tileSetPath = tmxFile.substr(0, dirEndPos + 1) + tileSetPath;

        // sanity check -> valid tileset file
        tinyxml2::XMLDocument tileSetDoc;
        if (tileSetDoc.LoadFile(tileSetPath.c_str()))
        {
            PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogFile, "Unable to load file: %s", tileSetPath.c_str());
            return false;
        }

        //----------------------------------------------------------------------------------------------------
        // Parsing tile set doc
        //----------------------------------------------------------------------------------------------------
        TileSet tileSet;
        tinyxml2::XMLElement* pTileSetRoot = tileSetDoc.RootElement();
        tileSet.m_name = pTileSetRoot->Attribute("name");
        tileSet.m_firstGid = pTileSetElement->IntAttribute("firstgid");
        tileSet.m_numOfColumns = pTileSetRoot->IntAttribute("columns");
        tileSet.m_tileWidth = pTileSetRoot->IntAttribute("tilewidth");
        tileSet.m_tileHeight = pTileSetRoot->IntAttribute("tileheight");

        // grab image element and parse info
        tinyxml2::XMLElement* pImageElement = pTileSetRoot->FirstChildElement("image");
        std::string imgPath = pImageElement->Attribute("source");

        // sanity check -> is the image in the same directory?
        std::size_t imgPos = imgPath.find_last_of('/');
        std::string srcImgName;
        if (imgPos == std::string::npos)
        {
            // build the correct directory string
            std::size_t tsxDirEndPos = tileSetPath.find_last_of('\\');
            srcImgName = imgPath;
            imgPath = tileSetPath.substr(0, tsxDirEndPos + 1) + imgPath;
        }
        else
        {
            // walk imgPath for correct path
            std::size_t erasePos = 0;
            while (erasePos != imgPath.length())
            {
                // break out if next character isn't a '.', '\\' and '/'
                if (imgPath[erasePos + 1] != '.' && imgPath[erasePos + 1] != '\\' && imgPath[erasePos + 1] != '/')
                    break;
                erasePos += 1;
            }
            imgPath.erase(0, erasePos + 1);
            srcImgName = imgPath.substr(imgPos - erasePos);
        }
        

        // find name of src image
        //std::string srcImgName = imgPath.substr(imgPos + 1);

        // broadcast load texture event
        m_textureEvent.m_textureName = srcImgName;
        m_textureEvent.m_textureFile = imgPath;
        m_textureEvent.m_eventStr = "Sending LoadTexture event!";
        m_textureEvent.m_type = Event::Type::kLoadTexture;
        EventSystem::BroadcastEvent(&m_textureEvent);

        // grab texture
        m_textureEvent.m_type = Event::Type::kRequestMapTexture;
        EventSystem::BroadcastEvent(&m_textureEvent);

        // set texture
        tileSet.m_pSrcTexture = m_textureEvent.m_pTexture;

        // add to container
        m_maps[m_currentMap].m_tileSets.emplace_back(tileSet);

        // iterate to next element
        pTileSetElement = pTileSetElement->NextSiblingElement("tileset");
    }

    //----------------------------------------------------------------------------------------------------
    // Parsing layer(s) from xml
    //----------------------------------------------------------------------------------------------------
    tinyxml2::XMLElement* pLayerElement = pMapRoot->FirstChildElement("layer");

    // create layer
    Layer layer; 
    layer.m_name = pLayerElement->Attribute("name");
    layer.m_width = pLayerElement->IntAttribute("width");
    layer.m_height = pLayerElement->IntAttribute("height");
    
    // grab data as one string
    std::string layerData = pLayerElement->FirstChildElement("data")->GetText();

    // calc size
    std::size_t layerSize = layer.m_width * layer.m_height;

    // shove into stream
    std::stringstream dataStream(layerData);
    dataStream.ignore(1); // ignore newline

    // grab lines
    for (std::size_t i = 0; i < layerSize; ++i)
    {
        // grab gid
        long long gid; // must be a long long here because tiled bit packs rotational information inside of gid
        dataStream >> gid;
        dataStream.ignore(1); // ignore comma

        ////
        //if (((int)i + 1) % layer.m_width == 0)
        //    dataStream.ignore(1); // ignore comma

        // sanity check -> valid gid
        if (gid == 0)
            continue;

        // TEST: Can we determine flips?
        bool horzFlip = gid & kHorzFlipFlag;
        bool vertFlip = gid & kVertFlipFlag; 
        bool diagFlip = gid & kDiagFlipFlag;
        gid &= ~ (kHorzFlipFlag | kVertFlipFlag | kDiagFlipFlag); // clear the bit flags
        
        // create tile and determine tileset
        Tile tile;
        tile.m_tileSetOwner = m_maps[m_currentMap].GetTileSet((int)gid);
        tile.m_gid = (int)gid;
        tile.m_relativeGid = tile.m_gid - tile.m_tileSetOwner.m_firstGid;
        tile.m_diagFlip = diagFlip;
        tile.m_horzFlip = horzFlip;
        tile.m_vertFlip = vertFlip;

        // setup for calculations
        int columns = tile.m_tileSetOwner.m_numOfColumns;
        int tileWidth = tile.m_tileSetOwner.m_tileWidth;
        int tileHeight = tile.m_tileSetOwner.m_tileHeight;

        // calc src rect
        int srcRectX = tile.m_relativeGid % columns;
        int srcRectY = tile.m_relativeGid / columns;

        // set src rect
        tile.m_srcRect = SDL_Rect{ (srcRectX * tileWidth), (srcRectY * tileHeight), tileWidth, tileHeight };

        // calc dest rect (world location)
        int destRectX = i % layer.m_width;
        int destRectY = int(i / layer.m_width); // ask why we use width here instead of height
        // set dest rect
        tile.m_destRect = SDL_Rect{ (destRectX * tileWidth), (destRectY * tileHeight), tileWidth, tileHeight };

        // add to tile container
        layer.m_tiles.emplace_back(tile);

        // create tile game object
        GameObject* pTile = m_pObjPool->CreateGameObject("tile").get();
        //pTile->SetComponentPool(m_pComponentPool);
        pTile->AttachComponent(ComponentPool::Type::kTexture);
        pTile->GetTextureComponent()->m_srcRect = tile.m_srcRect;
        pTile->GetTextureComponent()->m_pTexture = tile.m_tileSetOwner.m_pSrcTexture;
        pTile->SetPosition(Vector2D{ (float)destRectX * tileWidth * kPixelsPerMeter, (float)destRectY* tileHeight * kPixelsPerMeter });
        pTile->SetTransform(tile.m_destRect);
        pTile->SetSize(Vector2D{ (float)tile.m_destRect.w * kPixelsPerMeter , (float)tile.m_destRect.h * kPixelsPerMeter});
        pTile->SetDiagFlip(tile.m_diagFlip);
        pTile->SetHorzFlip(tile.m_horzFlip);
        pTile->SetVertFlip(tile.m_vertFlip);
        pTile->SetRenderFlag(true);
        View::Get()->AddRenderable(pTile);
        //m_tilesToRender.emplace_back(pTile);
    }

    //// send all tiles to render
    //m_mapEvent.m_type = Event::Type::kMap;
    //m_mapEvent.m_pTiles = &m_tilesToRender;
    //EventSystem::BroadcastEvent(&m_mapEvent);

    // add layer to map
    m_maps[m_currentMap].m_layers.emplace_back(layer);

    //----------------------------------------------------------------------------------------------------
    // Parsing collision object(s) from xml
    //----------------------------------------------------------------------------------------------------
    tinyxml2::XMLElement* pCollisionElement = pMapRoot->FirstChildElement("objectgroup"); // go to object group
    
    // sanity check -> valid object data?
    tinyxml2::XMLElement* pObjectElement = nullptr;
    if (pCollisionElement)
        pObjectElement = pCollisionElement->FirstChildElement("object"); // find first object

    // iterate over all objects
    while (pObjectElement)
    {
        // create collider
        Collider collider;
        collider.m_id = pObjectElement->IntAttribute("id");
        collider.m_type = pObjectElement->Attribute("type");
        collider.m_xPos = (int)pObjectElement->FloatAttribute("x");
        collider.m_yPos = (int)pObjectElement->FloatAttribute("y");
        collider.m_width = pObjectElement->IntAttribute("width");
        collider.m_height = pObjectElement->IntAttribute("height");
        collider.m_destRect = SDL_Rect{ int(collider.m_xPos * kPixelsPerMeter), int(collider.m_yPos * kPixelsPerMeter),
            int(collider.m_width * kPixelsPerMeter), int(collider.m_height * kPixelsPerMeter) };


        // add to map
        m_maps[m_currentMap].m_colliders.emplace_back(collider);


        

        // create collider game object
        GameObject* pCollider = m_pObjPool->CreateGameObject(collider.m_type).get();
        //pCollider->SetComponentPool(m_pComponentPool);
        pCollider->SetSize(Vector2D{ (float)collider.m_destRect.w, (float)collider.m_destRect.h });
        pCollider->SetPosition(Vector2D{ (float)collider.m_destRect.x, (float)collider.m_destRect.y });
        pCollider->SetTransform(collider.m_destRect);
        pCollider->AttachComponent(ComponentPool::Type::kPhysics);
        pCollider->GetPhysicsComponent()->CreateBody(PhysicsSystem::RigidBodyType::kStatic, pCollider->GetPosition().m_x, pCollider->GetPosition().m_y);
        pCollider->GetPhysicsComponent()->CreateFixture(PhysicsSystem::PolygonShapeType::kBox, pCollider->GetSize().m_x, pCollider->GetSize().m_y);

        // iterate to next element
        pObjectElement = pObjectElement->NextSiblingElement("object");
    }


    return true;
}

void MapSystem::CleanUp()
{
    //TODO: clean maps here
    for (TileMap& map : m_maps)
    {
        map.m_colliders.~vector();
        map.m_layers.~vector();
        map.m_tileSets.~vector();
    }
    m_maps.~vector();
}

MapSystem* MapSystem::Get()
{
    // sanity check -> is there already an instance?
    if (!m_pInstance)
        m_pInstance = new MapSystem();
    
    return m_pInstance;
}

void MapSystem::Delete()
{
    delete m_pInstance;
    m_pInstance = nullptr;
}

void MapSystem::HandleEvent(Event* pEvent)
{
    // handle event based on type
    switch (pEvent->m_type)
    {
        case Event::Type::kSendMapTexture:
        {
            // static cast
            TextureEvent* pTextureEvent = static_cast<TextureEvent*>(pEvent);

            // set event data
            m_textureEvent.m_pTexture = pTextureEvent->m_pTexture;
            
            break;
        }

        default:
        {
            break;
        }
    }
}

MapSystem::TileSet MapSystem::TileMap::GetTileSet(int gid)
{
    // search for tileset
    for (std::size_t i = 0; i < m_tileSets.size(); ++i)
    {
        if (gid < m_tileSets[i].m_firstGid)
            return m_tileSets[i - 1];
    }
    return m_tileSets[m_tileSets.size() - 1];
}
