#include "Application.h"
#include "../Utils/PhazeLogger.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <vld.h>
#include "../Logic/GameObjects/ObjectPool.h"
#include "../Logic/GameObjects/ComponentPool.h"
#include <lua.hpp> // TESTING
#include "../Scripting/RegisterLuaFunctions.h" //< to register generated lua functions
#include "../Logic/Physics/PhysicsSystem.h"
#include "../Views/MapSystem.h"

// TODO: Make this configurable
const std::string kLogOutputFile = "Logs/AppLog.txt";

bool Application::LuaInit()
{
    
    // init lua state
    m_pLuaState = luaL_newstate();

    // sanity check -> lua state loaded
    if (!m_pLuaState)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Lua State load failed!");
        return false;
    }

    // open lua libs
    luaL_openlibs(m_pLuaState);

    // register generated lua functions
    RegisterLuaFunctions(m_pLuaState);

    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Lua State and libs loaded!");
    return true;
}

// TODO: Make metrics data driven


const std::string kLuaScriptFilename = "Scripts/Lua/testScript.lua";

bool Application::InitSdl(const std::string& kWinName, const int kWinWidth, const int kWinHeight, const int kWinXPos, const int kWinYPos)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, SDL_GetError());
        return false;
    }

    m_pWindow = SDL_CreateWindow(kWinName.c_str(), kWinXPos, kWinYPos, kWinWidth, kWinHeight, 0);
    if (!m_pWindow)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, SDL_GetError());
        return false;
    }

    m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED | SDL_RendererFlags::SDL_RENDERER_PRESENTVSYNC);
    if (!m_pRenderer)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, SDL_GetError());
        return false;
    }


    //// TEST: using LUA to set image flags
    //luaL_dofile(m_pLuaState, kLuaScriptFilename.c_str()); // < load lua script
    //lua_getglobal(m_pLuaState, "SetImgFlags"); // < push the function to be called onto stack
    //lua_call(m_pLuaState, 0, 1); // < call SetImgFlags()
    //int imgFlag = (int)lua_tonumber(m_pLuaState, -1); // < cast returned value to an int as lua_tonumber() returns a double
    //lua_pop(m_pLuaState, 1); // < pop returned value off stack to maintain calling convention

    ////int imgFlag = IMG_INIT_PNG;

    //if (IMG_Init(imgFlag) != imgFlag)
    //{
    //    PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, IMG_GetError());
    //    return false;
    //}


    if (IMG_Init(IMG_INIT_PNG) == 0)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, IMG_GetError());
        return false;
    }

    if (TTF_Init() == -1)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, TTF_GetError());
        return false;
    }
    
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024) != 0)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, Mix_GetError());
        return false;
    }

    int soundFlags = MIX_INIT_MP3 | MIX_INIT_OGG;
    if (Mix_Init(soundFlags) != soundFlags)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, Mix_GetError());
        return false;
    }

    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "SDL Initialization complete!");
    return true;
}

int Application::LuaToCFuncTest(lua_State*)
{
    // create sound event
    SfxEvent m_sfxEvent;
    m_sfxEvent.m_sfxName = "clothSfx";
    m_sfxEvent.m_sfxFile = "Assets/Sounds/cloth-inventory.wav";
    m_sfxEvent.m_type = Event::Type::kLoadSfx;

    // broadcast request to load
    EventSystem::BroadcastEvent(&m_sfxEvent);

    // change event type
    m_sfxEvent.m_type = Event::Type::kPlaySfx;

    // broadcast request to play
    EventSystem::BroadcastEvent(&m_sfxEvent);
    
    PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Lua Func test complete!");
    return 0;
}



Application::Application(const std::string& kWinName, const int kWinWidth, const int kWinHeight, const int kWinXPos, const int kWinYPos)
{
    // setup lua and sdl
    LuaInit();
    InitSdl(kWinName, kWinWidth, kWinHeight, kWinXPos, kWinYPos);

    // Setup View
    View::Get()->SetRenderer(m_pRenderer);

    // TODO: allocate resource system here
    // allocate object pool
    m_pObjectPool = new ObjectPool;
    m_pObjectPool->Init();

    // pass object pool to other systems
    MapSystem::Get()->SetObjPool(m_pObjectPool);
    Game::Get()->SetObjPool(m_pObjectPool);

    // allocate component pool
    m_pComponentPool = new ComponentPool;
    m_pComponentPool->Init();
    m_pObjectPool->SetComponentPool(m_pComponentPool);

    // pass component pool to other systems
    MapSystem::Get()->SetComponentPool(m_pComponentPool);
    Game::Get()->SetComponentPool(m_pComponentPool);

    // create physics world
    PhysicsSystem::Get()->CreatePhysicsWorld( b2Vec2{ 0.f, 0.f } );

    // log successful init
    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Application Created!");
}

Application::~Application()
{
    // clean input system
    InputSystem::Get()->Destroy();
    //m_inputSys.CleanUp();
    //delete m_pInputSys;

    // clean game & view
    Game::Get()->Destroy();
    View::Get()->Destroy();
    //m_game.CleanUp();
    //delete m_pGame;

    // clean views
    //m_view.CleanUp();
    //delete m_pView;

    MapSystem::Get()->CleanUp();
    MapSystem::Get()->Delete();
    PhysicsSystem::Get()->Cleanup();
    PhysicsSystem::Get()->Delete();

    // TODO: clean resource system here
    // clean component pool
    delete m_pComponentPool;
    m_pComponentPool = nullptr;

    // clean obj pool
    delete m_pObjectPool;
    m_pObjectPool = nullptr;


    // clean sdl
    Mix_CloseAudio();
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);
    SDL_Quit();

    // clean lua
    lua_close(m_pLuaState);
}

PHAZE_API bool Application::LoadInputData(const std::string& filePath)
{
    return InputSystem::Get()->LoadFromXML(filePath);
}

void Application::Foo(int n)
{
    PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Lua exposition test. Param: %d", n);
}

void Application::RunApp()
{
    // TODO: Use std::chrono for more finite timing
    // time since last frame
    uint32_t lastTime_ms = SDL_GetTicks();

    // game loop
    while (Game::Get()->GetPlayingFlag())
    {
        // event loop
        SDL_Event sdlEvent;

        while (SDL_PollEvent(&sdlEvent))
        {
            if (sdlEvent.type == SDL_QUIT)
            {
                Game::Get()->QuitGame();
            }

            if(sdlEvent.type == SDL_KEYDOWN || SDL_KEYUP)
            {
                if (sdlEvent.type == SDL_KEYDOWN)
                {
                    //// send to lua Input()...
                    //luaL_dofile(m_pLuaState, kLuaScriptFilename.c_str()); // < load lua script
                    //lua_getglobal(m_pLuaState, "Input"); // < push the function to be called onto stack
                    //lua_pushinteger(m_pLuaState, sdlEvent.key.keysym.sym); // < push the key as an integer to Input() as the first parameter
                    //lua_register(m_pLuaState, "LuaToCFuncTest", LuaToCFuncTest); // < register function with lua state
                    //lua_call(m_pLuaState, 1, 0); // < call Input()

                }
                
                // send to input system
                InputSystem::Get()->HandleKeyInput(sdlEvent);
            }
        }

        // TODO: Use QueryPerformanceCounter() and QueryPerfomranceFrequence() instead
        // current frame time
        uint32_t elapsed_ms = SDL_GetTicks();

        // difference between current time and last frame time
        m_deltaTime_sec = ((float)(elapsed_ms - lastTime_ms)) / 1000.f;

        Game::Get()->Update(m_deltaTime_sec);
        View::Get()->Render();

        // update last frame time
        lastTime_ms = elapsed_ms;
    }
}

bool Application::LoadMap(const std::string& tmxFile)
{
    return MapSystem::Get()->LoadMap(tmxFile);
}
