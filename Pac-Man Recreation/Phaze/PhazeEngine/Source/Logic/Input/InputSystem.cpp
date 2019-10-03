#include "InputSystem.h"
#include "../GameObjects/GameObject.h"
#include "../../Utils/PhazeLogger.h"
#include "../../Utils/tinyxml2.h"
#include <SDL_events.h>

// TODO: data drive
const std::string kLogOutputFile = "Logs/InputSystemLog.txt";

// static defines
InputSystem* InputSystem::m_pInstance = nullptr;

void InputSystem::HandleKeyInput(SDL_Event& keyEvent)
{
    switch (keyEvent.type)
    {
        case SDL_KEYDOWN:
        {
            //KeyCommand keyCommand = m_pInputInterpreter.InterpretKeyInput(keyEvent.key);
            KeyCommand* pKeyCommand = GetKeyCommandByKey((const char)(keyEvent.key.keysym.sym));
            if (pKeyCommand)
            {
                pKeyCommand->SetState(KeyCommand::State::kPressed);
                pKeyCommand->Execute();
            }
            //m_pGame->HandleKeyCommand(m_pCommandFactory->CreateKeyboardCommand(keyEvent.key));
            PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Key Down");
            break;
        }

        case SDL_KEYUP:
        {
            //KeyCommand keyCommand = m_pInputInterpreter.InterpretKeyInput(keyEvent.key);
            KeyCommand* pKeyCommand = GetKeyCommandByKey((const char)(keyEvent.key.keysym.sym));
            if (pKeyCommand)
            {
                pKeyCommand->SetState(KeyCommand::State::kReleased);
                pKeyCommand->Execute();
            }
            //m_pGame->HandleKeyCommand(m_pCommandFactory->CreateKeyboardCommand(keyEvent.key));
            PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "Key Up");
            break;
        }

        default:
        {
            break;
        }
    }
}

bool InputSystem::LoadFromXML(const std::string& filePath)
{
    // sanity check->did we load the file ?
    tinyxml2::XMLDocument inputDoc;
    if (inputDoc.LoadFile(filePath.c_str()) != 0)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "Unable to load file: %s!", filePath.c_str());
        return false;
    }

    // check for keyboard and mouse elements
    bool parseKeyData = false;
    tinyxml2::XMLElement* pRoot = inputDoc.RootElement();
    const std::string test = pRoot->Name();
    if (test == "keyboard")
        parseKeyData = true;


    //----------------------------------------------------------------------------------------------------
    // Parsing keyboard input from xml
    //----------------------------------------------------------------------------------------------------
    tinyxml2::XMLElement* pKeyElement = nullptr;
    if (parseKeyData)
        pKeyElement = pRoot;

    const tinyxml2::XMLAttribute* pKeyAttribute = pKeyElement->FirstAttribute();
    while (pKeyAttribute)
    {
        const char* pKey = pKeyAttribute->Value();
        const char* pAttributeName = pKeyAttribute->Name();
        KeyCommand* pKeyCommand = new KeyCommand(pAttributeName, pKey[0]);
        m_keyCommands.emplace_back(pKeyCommand);
        //PHAZE_LOG(PhazeLogger::LogCategory::kWarning, kLogOutputFile, "Key Command: %s created!", pAttributeName);

        // move to next attribute
        pKeyAttribute = pKeyAttribute->Next();
    }
    return true;
}

void InputSystem::CleanUp()
{

}

InputSystem::~InputSystem()
{
    for (KeyCommand* pKeyCommand : m_keyCommands)
        delete pKeyCommand;
    m_keyCommands.clear();
}

InputSystem* InputSystem::Get()
{
    if (!m_pInstance)
    {
        m_pInstance = new InputSystem();
        PHAZE_LOG(PhazeLogger::LogCategory::kInfo, kLogOutputFile, "InputSystem created!");
    }
    
    return m_pInstance;
}

void InputSystem::Destroy()
{
    if (m_pInstance)
    {
        delete m_pInstance;
        m_pInstance = nullptr;
    }
}

KeyCommand* InputSystem::GetKeyCommandByKey(const char key)
{
    for (KeyCommand* pKeyCommand : m_keyCommands)
    {
        if (pKeyCommand->GetKey() == key)
            return pKeyCommand;
    }

    return nullptr;
}

KeyCommand* InputSystem::GetKeyCommandByName(const std::string& name)
{
    for (KeyCommand* pKeyCommand : m_keyCommands)
    {
        if (pKeyCommand->GetName() == name)
            return pKeyCommand;
    }
    
    return nullptr;
}

KeyCommand::KeyCommand(const std::string& name, SDL_Keycode key, State state)
    : m_name(name)
    , m_state(state)
    , m_key(key)
{
}

void KeyCommand::BindDelegate(std::function<void(KeyCommand*)> delegate)
{
    m_action = delegate;
}
