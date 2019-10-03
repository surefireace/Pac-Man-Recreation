#pragma once
// Created by Michael Smith
/// \file InputSystem.h

#include <string>
//#include <map>
#include <vector>
#include "../../Utils/Macros.h"
#include <functional>

// fwd declares
union SDL_Event;
class GameObject;

// typedefs
typedef int32_t SDL_Keycode;

/// \class KeyCommand
/// \brief Represents a keyboard command
class KeyCommand
{
public:
    /// \brief Different key states
    enum class State
    {
        kInvalid,
        kPressed,
        kReleased,
        kNumOfStates
    };

    /// Constructor
    /// \param name Name of the command
    /// \param state Command state
    /// \param key Key this command represents
    KeyCommand(const std::string& name, SDL_Keycode key, State state = State::kInvalid);

private:
    /// \brief Command state
    State m_state;

    /// \brief Name of the command
    std::string m_name;

    /// \brief Command key
    SDL_Keycode m_key;

    /// \brief Delegate to call when this command is invoked
    std::function<void(KeyCommand*)> m_action;
public:
    // accessors and mutators

    /// \brief Returns the state
    State GetState() { return m_state; }

    /// \brief Sets command state
    /// \param state State to be set
    void SetState(State state) { m_state = state; }

    /// \brief Binds a function delegate
    /// \param delegate The callback function to bind
    PHAZE_API void BindDelegate(std::function<void(KeyCommand*)> delegate);

    /// \brief Runs the callback function this command is bound to.
    void Execute() { if (m_action) m_action(this); };

    /// \brief Returns the name of the command
    const std::string GetName() { return m_name; };

    /// \brief Returns the key bound to this command
    SDL_Keycode GetKey() { return m_key; };
};


/// \class InputSystem
/// \brief Handles sdl input events then, tells InputInterpreter to interpret, then sends the interpreted input to Game
class InputSystem
{
public:
    // --------------------------------------------------------------------- //
    // Public Member Variables
    // --------------------------------------------------------------------- //


    // --------------------------------------------------------------------- //
    // Public Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Handles key input
    void HandleKeyInput(SDL_Event& keyEvent);

    /// \brief Loads input bindings from an xml file
    /// \param filePath The path of the xml file to be loaded.
    /// \return Returns false if xml load failed. Otherwise returns true
    bool LoadFromXML(const std::string& filePath);

    /// \brief Cleans input system from memory
    void CleanUp();
private:
    // --------------------------------------------------------------------- //
    // Private Member Variables
    // --------------------------------------------------------------------- //

    /// \brief Instance of this system
    static InputSystem* m_pInstance;

    /// \brief map of key commands
    //std::map<char, KeyCommand*> m_keyCommands;

    /// \brief vector of key commands
    std::vector<KeyCommand*> m_keyCommands;

    // --------------------------------------------------------------------- //
    // Private Member Functions
    // --------------------------------------------------------------------- //

    /// \brief Constructor
    InputSystem() {};

    /// \brief Destructor
    ~InputSystem();

public:
    // --------------------------------------------------------------------- //
    // Accessors & Mutators
    // --------------------------------------------------------------------- //

    /// \brief Retrives the input system
    /// \return Returns the instance of the system
    static PHAZE_API InputSystem* Get();

    /// \brief Destroys the input system
    static void Destroy();

    /// \brief Retrives a key command using the key to find it
    /// \brief key Key of the command to get
    /// \return Returns the requested command if it exists. Otherwise returns nullptr
    KeyCommand* GetKeyCommandByKey(const char key);

    /// \brief Retrives a key command using its name to find it
    /// \brief key Key of the command to get
    /// \return Returns the requested command if it exists. Otherwise returns nullptr
    PHAZE_API KeyCommand* GetKeyCommandByName(const std::string& name);
};