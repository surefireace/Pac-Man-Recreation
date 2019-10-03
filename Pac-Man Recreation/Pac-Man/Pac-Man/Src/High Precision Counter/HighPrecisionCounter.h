// HighPrecisionCounter.h
#pragma once

#include <chrono>

namespace Framework{

class HighPrecisionCounter
{
    typedef std::chrono::high_resolution_clock HighResolutionClock;
    typedef HighResolutionClock::time_point TimePoint;

    TimePoint m_startTime;
    bool m_started;

public:
    HighPrecisionCounter();
    void ResetCounter();
    double GetDeltaFromStart();
    bool Started(); // returns true if not started
    void Restart() { m_started = false;}    // resets the started flag
};

}  // end namespace Framework
