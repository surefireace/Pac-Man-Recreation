#include "PhazeLogger.h"
#include <fstream>
#include <sstream>
#include <direct.h>
#include <iostream>
#include <Windows.h> // for console color

// TODO: Use macros to make logging API cleaner
// TODO: Make categories customizable
// static definitions
const std::string PhazeLogger::m_categories[static_cast<int>(LogCategory::kNumOfCategories)] =
{ "[Info]", "[Warning]", "[Critical]" };

void* PhazeLogger::s_pStdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
const static std::string s_kLogDir = "Logs";

void PhazeLogger::WriteToLog(const LogCategory category, const std::string& sourceFileName, const std::string& destFileName, unsigned int lineNumber, const char* pMsg, ...)
{
    // handle variadic args
    va_list args;
    va_start(args, pMsg);

    // our msg buffer
    char buffer[1024];

    // store inside of the buffer
    vsprintf_s(buffer, pMsg, args);

    va_end(args);


    // find the file name
    std::size_t pos = sourceFileName.find_last_of('\\');
    std::string token = sourceFileName.substr(pos + 1);

    // make a log directory
    _mkdir(s_kLogDir.c_str());

    std::ofstream file;
    file.open(destFileName, std::ios::app);

    // write to the file
    std::ostringstream oss;
    oss << m_categories[static_cast<int>(category)] << '@' << token << " line: " << lineNumber << " message: " << buffer << '\n';
    file << oss.str();
    file.close();

    // store the default text color
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(s_pStdHandle, &consoleInfo);

    // check the category
    switch (category)
    {
        // prints yellow text
        case LogCategory::kWarning:
        {
            SetConsoleTextAttribute(s_pStdHandle, FOREGROUND_RED | FOREGROUND_GREEN);
            std::cout << oss.str();
            SetConsoleTextAttribute(s_pStdHandle, consoleInfo.wAttributes);
            break;
        }
        // prints red text and crashes
        case LogCategory::kCritical:
        {
            SetConsoleTextAttribute(s_pStdHandle, FOREGROUND_RED);
            std::cout << oss.str();
            SetConsoleTextAttribute(s_pStdHandle, consoleInfo.wAttributes);
            abort();
            break;
        }
        // unhandled
        default:
        {
            break;
        }
    }
}