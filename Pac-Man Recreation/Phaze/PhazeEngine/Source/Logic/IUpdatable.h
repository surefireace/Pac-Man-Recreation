#pragma once

/// \file IUpdatable.h

/// \class IUpdatable
/// \brief Interface for anything that updates
class IUpdatable
{
public:
    /// \brief Update interface function
    /// \param delatTime_sec Time difference between frames in seconds
    virtual void Update(float deltaTime_sec) = 0;
};