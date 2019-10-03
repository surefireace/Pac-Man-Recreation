// Made by Donovan Colen
#include "World.h"
#include "XML/XmlReader.h"
#include <iostream>
#include <fstream>
#include "GameObjects\PlayerBase\Ghosts\PinkyGhost.h"
#include "GameObjects\PlayerBase\Ghosts\InkyGhost.h"
#include "GameObjects\PlayerBase\Ghosts\ClydeGhost.h"


World* World::s_pInst = nullptr;    // initialize the pointer for singleton

World::World()
    : m_map("Assets\\Map\\Pac-Man_map.tmx")
    , m_inputBindings("Data\\Input\\Bindings.xml")
    , m_windowName("Pac-Man")
    , m_font("Assets\\Fonts\\impact.ttf")
    , m_afraidPath("Assets\\Sprites\\Ghost Fear_blue.png")
    , m_eattenPath("Assets\\Sprites\\Ghost Eyes.png")
    , m_fontSize(24)
    , m_windowWidth(448)
    , m_windowHeight(576)
    , m_windowOffsetX(100)
    , m_windowOffsetY(100)
    , m_dotCount(0)
    , m_speed(2)
    , m_scatterDur(7)
    , m_chaseDur(20)
    , m_frightenedDur(8)
    , m_startDelay(4)
    , m_powerDotsEatten(0)
	, m_PacMan(nullptr)
	, m_gameOver(false)
	, m_highScore(0)
	, m_lastMode(Ghost::GhostModes::kNone)
{

}

World::~World()
{
    delete m_pPlayer;
    for (int i = 0; i < s_kNumGhosts; i++)
    {
        delete m_pGhosts[i];
    }

    for (int i = 0; i < s_kNumDots; i++)
    {
        delete m_pDots[i];
    }

    for (int i = 0; i < s_kNumPowerDots; i++)
    {
        delete m_pPowerDots[i];
    }

    for (int i = 0; i < s_kMaxLives; i++)
    {
        delete m_lifeSprites[i];
    }

    delete m_pFruit;

    delete m_PacMan;
}

// get the singleton
World* World::GetInstance()
{
    if (s_pInst == nullptr)
    {
        s_pInst = new World();
    }
    return s_pInst;
}

void World::Init()
{
    m_PacMan = new Application(m_windowName, m_windowWidth, m_windowHeight, m_windowOffsetX, m_windowOffsetY);
    m_PacMan->LoadMap(m_map);
    LoadDots();
    LoadUI();
    LoadSounds();
    LoadHighScore();

    m_pPlayer = new Player();
    m_pPlayer->Init((int)s_kPlayerStart.m_x, (int)s_kPlayerStart.m_y, s_kImageSize, s_kImageSize, m_speed, "Assets\\Sprites\\Pac-Man mouth open.png", "player");  // pac man

    m_PacMan->LoadInputData(m_inputBindings);
    Game::Get()->BindKey("QuitGame", &Game::QuitGameByKey);

    // Seting up the bindings
    m_pPlayer->BindKey("MoveLeft", &Player::Move);
    m_pPlayer->BindKey("MoveRight", &Player::Move);
    m_pPlayer->BindKey("MoveUp", &Player::Move);
    m_pPlayer->BindKey("MoveDown", &Player::Move);

    LoadGhosts();

    // correct the number of lives rendered
    for (int i = 0; i < s_kMaxLives; i++)
    {
        if (i >= m_pPlayer->GetLivesLeft())
        {
            m_lifeSprites[i]->SetRenderFlag(false);
        }
    }

    // add the world so it has its update called
    Game::Get()->AddUpdatable(World::GetInstance());
    
    m_startTimer.ResetCounter();
    m_PacMan->RunApp();
}

void World::Update(const float deltaTime_sec)
{
    if (!m_gameOver)
    {
        if (m_startTimer.GetDeltaFromStart() >= m_startDelay)
        {
            CheckConditions();
            Vector2D playLoc = m_pPlayer->GetTileLocation();
            for (int i = 0; i < s_kNumGhosts; i++)
            {
                Vector2D ghostLoc = m_pGhosts[i]->GetTileLocation();
                if (ghostLoc.m_x == playLoc.m_x && ghostLoc.m_y == playLoc.m_y)
                {
                    if (m_pGhosts[i]->CanBeEatten())
                    {
                        m_pGhosts[i]->Eatten();
                    }
                    else
                    {
                        if (m_pPlayer->GetLivesLeft() > 0)
                        {
                            m_pPlayer->LoseLife();

                            // correct the number of lives rendered
                            for (int i = 0; i < s_kMaxLives; i++)
                            {
                                if (i >= m_pPlayer->GetLivesLeft())
                                {
                                    m_lifeSprites[i]->SetRenderFlag(false);
                                }
                            }
                            Reset();
                            return;
                        }
                        else
                        {
                            m_gameOver = true;
                        }
                    }
                }
                m_pGhosts[i]->Update(deltaTime_sec);
            }

            m_pPlayer->Update(deltaTime_sec);

            // update UI
            m_scoreField.m_text = "Score: " + std::to_string(m_pPlayer->GetScore());
            UISystem::Get()->AddTextField("score", m_scoreField);

            if (m_pPlayer->GetScore() > m_highScore)
            {
                m_highScore = m_pPlayer->GetScore();
                m_highScoreField.m_text = "High Score: " + std::to_string(m_highScore);
                UISystem::Get()->AddTextField("highScore", m_highScoreField);
            }
        }
    }
    else
    {
        SaveHighScore();
        int dotsEaten = s_kNumDots - m_dotCount;
        if (dotsEaten == s_kNumDots && m_powerDotsEatten == s_kNumPowerDots)
        {
            NewLevel();
        }
        else
        {
            UISystem::Get()->AddTextField("gameOver", m_gameOverField);

            m_ghostSound.m_type = Event::Type::kStopMusic;
            EventSystem::BroadcastEvent(&m_ghostSound);

            m_ghostAfraidSound.m_type = Event::Type::kStopMusic;
            EventSystem::BroadcastEvent(&m_ghostAfraidSound);


        }
    }
}

// returns true if the tile can be move in
bool World::IsValidTile(int id, bool canEnterHouseFlag)
{
    if (canEnterHouseFlag)  // check to see if the ghost house doors are vaid tile id
    {
        if (id == 10 || id == 39 || id == 34 || id == s_kTunnelTileID || id == 4)
        {
            return true;
        }

    }
    else
    {
        if (id == 10 || id == 39 || id == 34 || id == s_kTunnelTileID)
        {
            return true;
        }
    }
    return false;
}

// returns the tile at that location
Tile * World::GetTile(int x, int y)
{
    // take the tunnel tiles into account
    if (y == s_kTunnelTileY && (x >= 27 || x <= 0))
    {
        for (int i = 0; i < s_kNumTunnelTiles; i++)
        {
            if (m_pTunnelTiles[i]->GetX() == x)   // check if the tile is part of the tunnel
            {
                return m_pTunnelTiles[i];
            }
        }
    }

    return m_pTiles[(s_kTileRow * y) + x];
}

// returns the tiles around the tile position given that they are a valid move locations and the directions
std::map<Tile*, Vector2D> World::GetValidNeighbors(int x, int y, bool canEnterHouseFlag)
{
    std::map<Tile*, Vector2D> validTiles;
    Tile* tempTile;
    bool inTunnel = false;

    if (y == s_kTunnelTileY)
    {
        for (int i = 0; i < s_kNumTunnelTiles; i++)
        {
            // right
            if (m_pTunnelTiles[i]->GetX() == x + 1)   // check if the tile is part of the tunnel
            {
                validTiles.emplace(m_pTunnelTiles[i], Vector2D{ 1, 0 }); // add the tile 
            }
            else if(m_pTunnelTiles[i]->GetX() == x - 1) // left
            {
                validTiles.emplace(m_pTunnelTiles[i], Vector2D{ -1, 0 }); // add the tile 
            }
        }

        inTunnel = !validTiles.empty();
        
    }

    if (!inTunnel)  // no need to check up and down in they entered the tunnel
    {
        // up
        tempTile = m_pTiles[(s_kTileRow * (y - 1)) + x];
        if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
        {
            validTiles.emplace(tempTile, Vector2D{ 0, -1 });
        }
        // down
        tempTile = m_pTiles[(s_kTileRow * (y + 1)) + x];
        if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
        {
            validTiles.emplace(tempTile, Vector2D{ 0, 1 });
        }
    }

    // left
    tempTile = m_pTiles[(s_kTileRow * y) + (x - 1)];
    if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
    {
        validTiles.emplace(tempTile, Vector2D{ -1, 0 });
    }
    // right
    tempTile = m_pTiles[(s_kTileRow * y) + (x + 1)];
    if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
    {
        validTiles.emplace(tempTile, Vector2D{ 1, 0 });
    }

    return validTiles;
}

// returns a vector of valid move directions
std::vector<Vector2D> World::GetValidDirections(int x, int y, bool canEnterHouseFlag)
{
    std::vector<Vector2D> validDirs;
    Tile* tempTile;
    bool inTunnel = false;

    if (y == s_kTunnelTileY)
    {
        for (int i = 0; i < s_kNumTunnelTiles; i++) // check to see if it is a tunnel tile
        {
            if (m_pTunnelTiles[i]->GetX() == x + 1) // right
            {
                validDirs.emplace_back(Vector2D{ 1, 0 });
            }
            else if (m_pTunnelTiles[i]->GetX() == x - 1)    //left
            {
                validDirs.emplace_back(Vector2D{ -1, 0 });
            }
        }

        inTunnel = !validDirs.empty();
    }

    if (!inTunnel) // no need to check up and down in they entered the tunnel
    {
        // up
        tempTile = m_pTiles[(s_kTileRow * (y - 1)) + x];
        if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
        {
            validDirs.emplace_back(Vector2D{ 0, -1 });  // up
        }
        // down
        tempTile = m_pTiles[(s_kTileRow * (y + 1)) + x];
        if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
        {
            validDirs.emplace_back(Vector2D{ 0, 1 });   // down
        }
    }

    // left
    tempTile = m_pTiles[(s_kTileRow * y) + (x - 1)];
    if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
    {
        validDirs.emplace_back(Vector2D{ -1, 0 });  //left
    }
    // right
    tempTile = m_pTiles[(s_kTileRow * y) + (x + 1)];
    if (IsValidTile(tempTile->GetId(), canEnterHouseFlag))
    {
        validDirs.emplace_back(Vector2D{ 1, 0 });   //right
    }

    return validDirs;
}

// updates the number of dots eatten
void World::DotEatten()
{
    // play sound when dot is eatten
    EventSystem::BroadcastEvent(&m_eatSound);
    m_dotCount--;
}

// sets ghosts to frightened mode
void World::EmpowerPacMan()
{
    m_ghostSound.m_type = Event::Type::kStopMusic;
    EventSystem::BroadcastEvent(&m_ghostSound);

    m_ghostAfraidSound.m_type = Event::Type::kPlayMusic;
    EventSystem::BroadcastEvent(&m_ghostAfraidSound);

    m_gameTimer.ResetCounter();
    m_powerDotsEatten++;

    for (int i = 0; i < s_kNumGhosts; i++)
    {
        if (m_pGhosts[i]->CurrentMode() != Ghost::GhostModes::kFrighten)
        {
            if (m_lastMode != m_pGhosts[i]->CurrentMode())
            {
                m_lastMode = m_pGhosts[i]->CurrentMode();
            }
            m_pGhosts[i]->SetMode(Ghost::GhostModes::kFrighten);
        }
    }
}

// loads the dots into the world
void World::LoadDots()
{
    //Create texture event
    TextureEvent textureEventPower;

    //Set its attributes
    textureEventPower.m_type = Event::Type::kLoadTexture; // set the type
    textureEventPower.m_textureName = "power"; // name of the texture
    textureEventPower.m_textureFile = "Assets\\Sprites\\Power Dot.png"; // filepath of texture
    textureEventPower.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&textureEventPower);

    //Create texture event
    TextureEvent textureEventDot;

    //Set its attributes
    textureEventDot.m_type = Event::Type::kLoadTexture; // set the type
    textureEventDot.m_textureName = "dot"; // name of the texture
    textureEventDot.m_textureFile = "Assets\\Sprites\\Dot.png"; // filepath of texture
    textureEventDot.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&textureEventDot);

    // find and get the map tile id's
    XmlReader reader;
    tinyxml2::XMLElement* root = reader.Load("Assets\\Map\\Pac-Man_map.tmx");
    vector<tinyxml2::XMLElement*> elements = reader.GetElements(root);
    elements = reader.GetElements(elements[1]);
    tinyxml2::XMLElement* data = elements[0];
    string text = data->GetText();

    // go through the map's tile id's to find the dot tiles
    int y = 0;
    int x = 0;
    int power = 0;
    int tilecount = 0;
    for (unsigned int i = 1; i < text.length(); i++)
    {
        if (text[i] == '\n')
        {
            // create the final tile
            if (i == text.length() - 1)
            {
                int id = atoi(&text[i - 2]);
                m_pTiles[tilecount] = new Tile(x, y, id);
            }
            x = 0;
            y++;
            continue;
        }
        else if (text[i] == ',')
        {
            if (text[i - 2] == ',')
            {
                m_pTiles[tilecount] = new Tile(x, y, atoi(&text[i - 1]));
                tilecount++;
            }
            else
            {
                int id = atoi(&text[i - 2]);
                m_pTiles[tilecount] = new Tile(x, y, id);
                if (id == 10)
                {
                    m_pTiles[tilecount]->AddDot(m_pDots[m_dotCount - 1]);
                }
                else if(id == 39)
                {
                    m_pTiles[tilecount]->AddDot(m_pPowerDots[power - 1]);
                }
                tilecount++;
            }
            x++;
            continue;
        }
        else if (text[i] == '1')
        {
            if (text[i + 1] == '0')
            {
                m_pDots[m_dotCount] = new Dots();
                m_pDots[m_dotCount]->Init((s_kTileSize * x), (s_kTileSize * y), s_kTileSize, s_kTileSize, 0, textureEventDot, "dot");
                m_dotCount++;
            }

        }
        else if (text[i] == '3')
        {
            if (text[i + 1] == '9')
            {
                m_pPowerDots[power] = new PowerDots();
                m_pPowerDots[power]->Init((s_kTileSize * x), (s_kTileSize * y), s_kTileSize, s_kTileSize, 0, textureEventPower, "power");
                power++;
            }

        }
    }

    // to check if loaded the dots correctly
    if (m_dotCount != s_kNumDots)
    {
        cout << "error dots count wrong" << endl;
    }
    // to check if loaded the power dots correctly
    if (power != s_kNumPowerDots)
    {
        cout << "error power dots count wrong" << endl;
    }
    // check if the tiles loaded properly
    if (tilecount != (s_kTileCol * s_kTileRow) - 1)
    {
        cout << "error tile loading" << endl;
    }

    m_pTunnelTiles[0] = new Tile(-2, s_kTunnelTileY, s_kTunnelTileID);
    m_pTunnelTiles[1] = new Tile(-1, s_kTunnelTileY, s_kTunnelTileID);
    m_pTunnelTiles[2] = new Tile(28, s_kTunnelTileY, s_kTunnelTileID);
    m_pTunnelTiles[3] = new Tile(29, s_kTunnelTileY, s_kTunnelTileID);

    //Create texture event
    TextureEvent textureEventFruit;

    //Set its attributes
    textureEventFruit.m_type = Event::Type::kLoadTexture; // set the type
    textureEventFruit.m_textureName = "fruit"; // name of the texture
    textureEventFruit.m_textureFile = "Assets\\Sprites\\Cherry.png"; // filepath of texture
    textureEventFruit.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&textureEventFruit);
    m_pFruit = new Fruit();
    m_pFruit->Init((int)s_kFruitLoc.m_x, (int)s_kFruitLoc.m_y, s_kImageSize, s_kImageSize, 0, textureEventFruit, "fruit");

    int tileX = (int)s_kFruitLoc.m_x / s_kTileSize;
    int tileY = ((int)s_kFruitLoc.m_y / s_kTileSize) + 1;
    m_pTiles[(s_kTileRow * tileY) + tileX]->AddDot(m_pFruit);

    m_pFruit->Disable();    // the fruit doesnt spawn till later
}

// loads the ghosts
void World::LoadGhosts()
{
    m_pGhosts[0] = new Ghost(m_pPlayer);
    m_pGhosts[0]->Init((int)s_kBlinkyStart.m_x, (int)s_kBlinkyStart.m_y, s_kImageSize, s_kImageSize, m_speed, "Assets\\Sprites\\Blinky temp.png", "blinky");      // red ghost
    m_pGhosts[0]->SetScatterTarget(Vector2D{ 25, 0 });

    m_pGhosts[1] = new PinkyGhost(m_pPlayer);
    m_pGhosts[1]->Init((int)s_kPinkyStart.m_x, (int)s_kPinkyStart.m_y, s_kImageSize, s_kImageSize, m_speed, "Assets\\Sprites\\Pinky temp.png", "pinky");        // pink ghost
    m_pGhosts[1]->SetScatterTarget(Vector2D{ 2, 0 });

    m_pGhosts[2] = new InkyGhost(m_pPlayer, m_pGhosts[0]);
    m_pGhosts[2]->Init((int)s_kInkyStart.m_x, (int)s_kInkyStart.m_y, s_kImageSize, s_kImageSize, m_speed, "Assets\\Sprites\\Inky temp.png", "inky");          // blue ghost
    m_pGhosts[2]->SetScatterTarget(Vector2D{ 27, 35 });

    m_pGhosts[3] = new ClydeGhost(m_pPlayer);
    m_pGhosts[3]->Init((int)s_kClydeStart.m_x, (int)s_kClydeStart.m_y, s_kImageSize, s_kImageSize, m_speed, "Assets\\Sprites\\Clyde temp.png", "clyde");        // orange ghost
    m_pGhosts[3]->SetScatterTarget(Vector2D{ 0, 35 });

    TextureEvent afraidTexture;

    //Set its attributes 
    afraidTexture.m_type = Event::Type::kLoadTexture; // set the type
    afraidTexture.m_textureName = "afraid"; // name of the texture
    afraidTexture.m_textureFile = m_afraidPath; // filepath of texture
    afraidTexture.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&afraidTexture);

    //Now change the type of the event and set the game object that will have the texture
    afraidTexture.m_type = Event::Type::kRequestTexture;

    TextureEvent eattenTexture;

    //Set its attributes 
    eattenTexture.m_type = Event::Type::kLoadTexture; // set the type
    eattenTexture.m_textureName = "eatten"; // name of the texture
    eattenTexture.m_textureFile = m_eattenPath; // filepath of texture
    eattenTexture.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&eattenTexture);

    //Now change the type of the event and set the game object that will have the texture
    eattenTexture.m_type = Event::Type::kRequestTexture;

    for (int i = 0; i < s_kNumGhosts; i++)
    {
        m_pGhosts[i]->SetFrightendModeTextures(afraidTexture, eattenTexture);
    }

}

// loads the ui
void World::LoadUI()
{
    //Load font
    UISystem::Get()->LoadFont("impact", m_font, m_fontSize);

    // Select font! By default no font is selected and only one font can be selected at a time!
    UISystem::Get()->SelectFont("impact");

    //set the text field text, color, x and y.
    m_scoreField.m_text = "Score: 0";
    m_scoreField.m_textColor = SDL_Color{ 255, 255, 255 }; // white text
    m_scoreField.m_fieldColor = SDL_Color{ 0, 0, 0 }; // black background
    m_scoreField.m_destRect.x = 10;
    m_scoreField.m_destRect.y = 10;

    //set the text field text, color, x and y.
    m_highScoreField.m_text = "High Score: 0";
    m_highScoreField.m_textColor = SDL_Color{ 255, 255, 255 }; // white text
    m_highScoreField.m_fieldColor = SDL_Color{ 0, 0, 0 }; // black background
    m_highScoreField.m_destRect.x = (s_kTileRow * s_kTileSize) / 2;
    m_highScoreField.m_destRect.y = 10;

    m_gameOverField.m_text = "Game Over";
    m_gameOverField.m_textColor = SDL_Color{ 0, 255, 0 }; // green text
    m_gameOverField.m_fieldColor = SDL_Color{ 0, 0, 0 }; // black background
    m_gameOverField.m_destRect.x = ((s_kTileRow * s_kTileSize) + (s_kTileSize * 4)) / 3;
    m_gameOverField.m_destRect.y = ((s_kTileCol * s_kTileSize) - (s_kTileSize * 3)) / 2;;


    //Add it to the UI System
    UISystem::Get()->AddTextField("score", m_scoreField);
    UISystem::Get()->AddTextField("highScore", m_highScoreField);

    //Create texture event
    TextureEvent textureEventLife;

    //Set its attributes
    textureEventLife.m_type = Event::Type::kLoadTexture; // set the type
    textureEventLife.m_textureName = "life"; // name of the texture
    textureEventLife.m_textureFile = "Assets\\Sprites\\Life.png"; // filepath of texture
    textureEventLife.m_eventStr = "Texture Event"; // this is optional mostly used for debugging

    //Broadcast it!
    EventSystem::BroadcastEvent(&textureEventLife);

    for (int i = 0; i < s_kMaxLives; i++)
    {
        m_lifeSprites[i] = new Dots();
        m_lifeSprites[i]->Init(0 + (i * s_kImageSize), ((s_kTileCol - 2) * s_kTileSize), s_kImageSize, s_kImageSize, 0, textureEventLife, "life");
    }

}

// loads the music and sounds
void World::LoadSounds()
{
    // some sound are music events because music events loop and sfx events do not loop
    //MUSIC
    // Create a music event and set its paramters
    m_ghostSound.m_musicFile = "Assets\\Sounds\\Pacman_Ghost_Sound.mp3";
    m_ghostSound.m_musicName = "ghost sound";
    m_ghostSound.m_type = Event::Type::kLoadMusic;

    //Broadcast it to load it!
    EventSystem::BroadcastEvent(&m_ghostSound);

    //Change the event type
    m_ghostSound.m_type = Event::Type::kPlayMusic;


    // Create a music event and set its paramters
    m_ghostAfraidSound.m_musicFile = "Assets\\Sounds\\Pacman_Blue_Ghost_Sound.mp3";
    m_ghostAfraidSound.m_musicName = "frightened mode sound";
    m_ghostAfraidSound.m_type = Event::Type::kLoadMusic;

    //Broadcast it to load it!
    EventSystem::BroadcastEvent(&m_ghostAfraidSound);

    //Change the event type
    m_ghostAfraidSound.m_type = Event::Type::kPlayMusic;


    //SFX
    //create the sfx event
    m_eatSound.m_sfxFile = "Assets\\Sounds\\Pacman_Eat_Sound.wav";
    m_eatSound.m_sfxName = "eatSound";
    m_eatSound.m_type = Event::Type::kLoadSfx;

    // brodcast to load
    EventSystem::BroadcastEvent(&m_eatSound);

    // change type
    m_eatSound.m_type = Event::Type::kPlaySfx;


    //sfx event set its paramters
    m_begginingMusic.m_sfxFile = "Assets\\Music\\pacman_beginning.wav";
    m_begginingMusic.m_sfxName = "beginning music";
    m_begginingMusic.m_type = Event::Type::kLoadSfx;

    //Broadcast it to load it!
    EventSystem::BroadcastEvent(&m_begginingMusic);

    //Change the event type
    m_begginingMusic.m_type = Event::Type::kPlaySfx;

    //Broadcast it to play it!
    EventSystem::BroadcastEvent(&m_begginingMusic);
}

// resets the ghost and pacman not the dots or score
void World::Reset()
{
    m_pPlayer->Reset((int)s_kPlayerStart.m_x, (int)s_kPlayerStart.m_y);
    m_pGhosts[0]->Reset((int)s_kBlinkyStart.m_x, (int)s_kBlinkyStart.m_y);
    m_pGhosts[1]->Reset((int)s_kPinkyStart.m_x, (int)s_kPinkyStart.m_y);
    m_pGhosts[2]->Reset((int)s_kInkyStart.m_x, (int)s_kInkyStart.m_y);
    m_pGhosts[3]->Reset((int)s_kClydeStart.m_x, (int)s_kClydeStart.m_y);

    m_pFruit->Disable();    // the fruit disapears even if not eaten
    m_ghostSound.m_type = Event::Type::kStopMusic;
    EventSystem::BroadcastEvent(&m_ghostSound);
    EventSystem::BroadcastEvent(&m_begginingMusic);
    m_startTimer.ResetCounter();
    m_gameTimer.Restart();

}

// checks to see if any other conditions have been met
void World::CheckConditions()
{

    if (!m_gameTimer.Started()) // start the game timer if havent started it yet
    {
        m_gameTimer.ResetCounter();
        m_ghostSound.m_type = Event::Type::kPlayMusic;
        EventSystem::BroadcastEvent(&m_ghostSound);

    }
    else
    {
        switch (m_pGhosts[0]->CurrentMode())
        {
        case Ghost::GhostModes::kChase:
            if (m_gameTimer.GetDeltaFromStart() >= m_chaseDur)
            {
                for (int i = 0; i < s_kNumGhosts; i++)
                {
                    m_pGhosts[i]->SetMode(Ghost::GhostModes::kScatter);
                }
                m_gameTimer.ResetCounter();
            }
            break;

        case Ghost::GhostModes::kScatter:
            if (m_gameTimer.GetDeltaFromStart() >= m_scatterDur)
            {
                for (int i = 0; i < s_kNumGhosts; i++)
                {
                    m_pGhosts[i]->SetMode(Ghost::GhostModes::kChase);
                }
                m_gameTimer.ResetCounter();
            }
            break;

        case Ghost::GhostModes::kFrighten:
            if (m_gameTimer.GetDeltaFromStart() >= m_frightenedDur)
            {
                m_ghostAfraidSound.m_type = Event::Type::kStopMusic;
                EventSystem::BroadcastEvent(&m_ghostAfraidSound);

                m_ghostSound.m_type = Event::Type::kPlayMusic;
                EventSystem::BroadcastEvent(&m_ghostSound);

                for (int i = 0; i < s_kNumGhosts; i++)
                {
                    m_pGhosts[i]->SetMode(m_lastMode);
                }
                m_gameTimer.ResetCounter();
            }
            break;

        default:
            printf("ERROR MODE!!!");
            break;

        }
    }

    int dotsEaten = s_kNumDots - m_dotCount;

    if (dotsEaten >= s_kInkyDots && m_pGhosts[2]->IsInHouse())
    {
        // inky leaves the house
        m_pGhosts[2]->NowLeaveHouse();
    }
    else if (dotsEaten >= s_kClydeDots && m_pGhosts[3]->IsInHouse())
    {
        // clyde leaves the house
        m_pGhosts[3]->NowLeaveHouse();
    }
    else if (dotsEaten >= 220)
    {
        if (m_pGhosts[0]->CurrentMode() != Ghost::GhostModes::kFrighten)
        {
            m_pGhosts[0]->SetMoveSpeed(m_speed * 1.1f);
        }
    }

    if (dotsEaten == s_kFruitDots[0])
    {
        m_pFruit->Reset();
    }
    if (dotsEaten == s_kFruitDots[1])
    {
        m_pFruit->Reset();
    }
    
    if (dotsEaten == s_kNumDots && m_powerDotsEatten == s_kNumPowerDots)    // check to see if the player won
    {
        m_gameOver = true;
    }
}

// save the game
void World::SaveHighScore()
{
    std::ofstream file;
    file.open("Save.txt");

    if (file.is_open())
    {
        file << m_highScore << std::endl;	// save the current wave number
        file.close();
    }
}

// check if there is a save and load it
void World::LoadHighScore()
{
    std::ifstream file;
    file.open("Save.txt");

    if (file.is_open())	// check if there is a file
    {
        std::string line;
        if (getline(file, line))	// check if the file has contents
        {
            m_highScore = std::stoi(line);	// set the high score
            m_highScoreField.m_text = "High Score: " + std::to_string(m_highScore);
            UISystem::Get()->AddTextField("highScore", m_highScoreField);

        }
    }
}

// starts a new level
void World::NewLevel()
{
    for (int i = 0; i < s_kNumDots; i++)
    {
        m_pDots[i]->Reset();
    }

    for (int i = 0; i < s_kNumPowerDots; i++)
    {
        m_pPowerDots[i]->Reset();
    }

    m_dotCount = s_kNumDots;
    m_powerDotsEatten = 0;

    for (int i = 0; i < s_kNumGhosts; i++)
    {
        m_pGhosts[i]->SetMode(Ghost::GhostModes::kScatter);
    }
    m_gameTimer.ResetCounter();

    Reset();

    m_gameOver = false;
}
