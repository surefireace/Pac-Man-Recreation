// Made by Donovan Colen
#pragma once
#include <string>
#include <vector>
#include <map>
#include <Application/Application.h>
#include "GameObjects/PlayerBase/PlayerBase.h"
#include "GameObjects/PlayerBase/Player.h"
#include "GameObjects/PlayerBase/Ghosts/Ghost.h"
#include "GameObjects/Dots.h"
#include "GameObjects/PowerDots.h"
#include "Logic/UI/UISystem.h"
#include "Tile.h"
#include "GameObjects/Fruit.h"
#include "High Precision Counter/HighPrecisionCounter.h"


static constexpr int s_kNumPowerDots = 4;
static constexpr int s_kNumDots = 240;
static constexpr int s_kTileSize = 16;
static constexpr int s_kTileRow = 28;
static constexpr int s_kTileCol = 36;
static constexpr int s_kNumTunnelTiles = 4;
static constexpr int s_kTunnelTileID = -1;
static constexpr int s_kTunnelTileY = 17;
static constexpr int s_kTunnelTileLX = -2;
static constexpr int s_kTunnelTileRX = 29;
static constexpr int s_kNumGhosts = 4;
static constexpr int s_kImageSize = 32;
static constexpr int s_kMaxLives = 5;
static constexpr int s_kInkyDots = 30;
static constexpr int s_kClydeDots = 90;
static constexpr int s_kCruiseElroyDots = 220;
static constexpr int s_kFruitDots[2] = { 70, 170 };
static constexpr float s_kTolerance = 3.0f;
static constexpr Vector2D s_kPlayerStart{ 208, 408 };
static constexpr Vector2D s_kBlinkyStart{ 208, 216 };
static constexpr Vector2D s_kPinkyStart{ 208, 264 };
static constexpr Vector2D s_kInkyStart{ 176, 264 };
static constexpr Vector2D s_kClydeStart{ 240, 264 };
static constexpr Vector2D s_kFruitLoc{ 208, 312 };



class World : public IUpdatable
{
private:
    // consts
    const std::string m_afraidPath;                     // path for the texture to use when ghosts are in frightened mode
    const std::string m_eattenPath;                     // path for the texture to use when ghosts eaten by pacman
    const std::string m_map;                            // the path and name to the map file
    const std::string m_inputBindings;                  // the path to the input bindings file
    const std::string m_windowName;                     // the name of the window
    const std::string m_font;                           // the path to the font
    const float m_speed;                                // the speed of the player and ghosts
    const float m_scatterDur;                           // the duration of scatter mode
    const float m_chaseDur;                             // the duration of chase mode
    const float m_frightenedDur;                        // the duration of Frightened mode
    const float m_startDelay;                           // the delay at the start of a reset and the game

    
    // default types
    int m_fontSize;                                     // the size of the font
    int m_windowWidth;                                  // the width of the window
    int m_windowHeight;                                 // the height of the window
    int m_windowOffsetX;                                // the window x offset
    int m_windowOffsetY;                                // the window y offset
    int m_dotCount;                                     // to keep track of number of dots left
    int m_powerDotsEatten;                              // to keep track of number of power dots eatten
    int m_highScore;                                    // holds the high score
    bool m_gameOver;                                    // to tell when the game is over
    

    // class types
    Application* m_PacMan;                              // the game engine pointer
    Player* m_pPlayer;                                  // the player aka pacman
    Ghost* m_pGhosts[s_kNumGhosts];                     // array for the ghosts
    PowerDots* m_pPowerDots[s_kNumPowerDots];           // array for the power dots
    Dots* m_pDots[s_kNumDots];                          // array for the dots
    Tile* m_pTiles[s_kTileRow * s_kTileCol];            // array for the tiles
    Tile* m_pTunnelTiles[s_kNumTunnelTiles];            // array for the tunnel tiles
    TextField m_scoreField;                             // the text field for the score
    TextField m_highScoreField;                         // the text field for the  highscore
    TextField m_gameOverField;                          // the text field for the  game over text
    SfxEvent m_begginingMusic;                          // the sound event for the begining music
    SfxEvent m_eatSound;                                // the sound event for pacman eating
    MusicEvent m_ghostSound;                            // the music event for the ghost sound
    MusicEvent m_ghostAfraidSound;                      // the music event for the ghost when in frightened mode
    Framework::HighPrecisionCounter m_startTimer;       // the timer to keep track of the start
    Framework::HighPrecisionCounter m_gameTimer;        // the timer to keep track of the game
    Ghost::GhostModes m_lastMode;                       // to keep track of the last mode the ghosts were in before frightened mode
    Dots* m_lifeSprites[s_kMaxLives];                   // UI sprites for the number of lives left. it is type Dots because it simplifies code
    Fruit* m_pFruit;                                    // the fruit that pacman can eat


    // simple constuctor. private for singleton
    World();

    // pointer for the location of the Singleton instance
    static World *s_pInst;

public:

    // simple deconstructor
    ~World();

    // get the singleton
    static World* GetInstance();

    // void funcs
    void Init();
    void Update(const float deltaTime_sec) override;
    void DotEatten();   // updates the number of dots eatten
    void EmpowerPacMan();   // sets ghosts to frightened mode

    // return type funcs
    bool IsValidTile(int id, bool canEnterHouseFlag = false);   // returns true if the tile can be move in
    Tile* GetTile(int x, int y); // returns the tile at that location
    std::map<Tile*, Vector2D> GetValidNeighbors(int x, int y, bool canEnterHouseFlag = false);  // returns the tiles around the tile position given that they are a valid move locations and the directions
    std::vector<Vector2D> GetValidDirections(int x, int y, bool canEnterHouseFlag = false);     // returns a vector of valid move directions
    int GetDotEattenCount() { return s_kNumDots - m_dotCount; } // returns the dots eatten count

    Application* GetApp() const { return m_PacMan; }    // returns the app pointer

private:

    void LoadDots();        // loads the dots into the world
    void LoadGhosts();      // loads the ghosts
    void LoadUI();          // loads the ui
    void LoadSounds();      // loads the music and sounds
    void Reset();           // resets the ghost and pacman not the dots or score
    void CheckConditions(); // checks to see if any other conditions have been met
    void SaveHighScore();   // save the highscore
    void LoadHighScore();   // loads the highscore if there is one
    void NewLevel();        // starts a new level
};