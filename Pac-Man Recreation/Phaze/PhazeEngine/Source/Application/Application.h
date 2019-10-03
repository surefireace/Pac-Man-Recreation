#pragma once
/// \file Application.h
#include "../Logic/Game.h"
#include "../Views/View.h"
#include "../Logic/Input/InputSystem.h"
#include "../Utils/Macros.h"
#include <string>

// fwd declares
struct SDL_Window;
struct SDL_Renderer;
class ObjectPool;
class ComponentPool;
struct lua_State;

/// \class Application
/// \brief Engine entry point
class Application
{
public:
    /// \brief Constructor. Initializes SDL as well.
    /// \param kWinName The name of the sdl window
    /// \param kWinWidth Window width
    /// \param kWinHeight Window height
    /// \param kWinXPos Window x position
    /// \param kWinYPos Window y position
    PHAZE_API Application(const std::string& kWinName = "Phaze_Game", const int kWinWidth = 800, const int kWinHeight = 600, const int kWinXPos = 50, const int kWinYPos = 50);

    /// \brief Destructor
    PHAZE_API ~Application();

    /// \brief Runs the application
    PHAZE_API void RunApp();

    /// \brief Loads a map from a given path
    /// \param tmxFile the filepath of the tmx(tileMap) file to load
    /// \return Returns true if map loaded successfully
    PHAZE_API bool LoadMap(const std::string& tmxFile);

    /// \brief Loads input bindings from a given path
    /// \param filePath The filepath of the input file(xml) to load
    /// \return Returns false if load failed. Otherwise returns true.
    PHAZE_API bool LoadInputData(const std::string& filePath);

    /// \brief Returns the delta time in seconds
    /// \return The delta time in seconds
    PHAZE_API const float GetDeltaTime() { return m_deltaTime_sec; }

    /// \brief TESTING expose to lua macro
    LuaFunction()
    void Foo(int n);
private:

    /// \brief Initializes SDL
    /// \param kWinName The name of the sdl window
    /// \param kWinWidth Window width
    /// \param kWinHeight Window height
    /// \param kWinXPos Window x position
    /// \param kWinYPos Window y position
    /// \return Returns false if sdl failed to inititialize
    bool InitSdl(const std::string& kWinName, const int kWinWidth, const int kWinHeight, const int kWinXPos, const int kWinYPos);

    /// \brief SDL Window
    SDL_Window* m_pWindow;

    /// \brief SDL Renderer
    SDL_Renderer* m_pRenderer;

    /// Game Object Pool.
    ObjectPool* m_pObjectPool;

    /// Component pool.
    ComponentPool* m_pComponentPool;

    /// \brief Lua state
    lua_State* m_pLuaState;

    /// \brief delta time in seconds
    float m_deltaTime_sec;

    /// \brief Initializes Lua state.
    bool LuaInit();

    /// \brief TESTING calling C++ functions from Lua.
    static int LuaToCFuncTest(lua_State* pLuaState);
};

