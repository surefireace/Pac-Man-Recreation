#pragma once

#include <string>
#include "Macros.h"

/// \file PhazeLogger.h


/// \brief Logging macro to make logging eaiser
/// \param category LogCategory of message
/// \param fileDestination Destination of log file
/// \param msg Message to log
#define PHAZE_LOG(category, fileDestination, msg, ...) PhazeLogger::WriteToLog(category, __FILE__, fileDestination, __LINE__, msg, __VA_ARGS__);

/// \class PhazeLogger
/// \brief Static class used to handle logging through engine and game
class PhazeLogger
{
public:
    /// \brief Log categories
    enum class LogCategory
    {
        kInfo,                      ///< Logged to file
        kWarning,                   ///< Logged to file, logged to console in yellow
        kCritical,                  ///< Logged to file, logged to console in red, will crash if set
        kNumOfCategories
    };

    /// Constructor
    /// Not ment to be invoked
    PhazeLogger() = delete;

    /// \brief Writes to logger
    PHAZE_API void static WriteToLog(const LogCategory category, const std::string& sourceFileName, const std::string& destFileName, unsigned int lineNumber, const char* pMsg, ...);
private:
    /// \brief Log category strings
    const static std::string m_categories[static_cast<int>(LogCategory::kNumOfCategories)];

    /// \brief Windows handle
    static void* s_pStdHandle;
};

