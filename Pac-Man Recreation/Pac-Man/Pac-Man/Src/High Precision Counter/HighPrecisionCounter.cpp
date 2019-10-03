// HighPrecisionCounter.cpp
#include "HighPrecisionCounter.h"

using namespace std::chrono;

namespace Framework{

HighPrecisionCounter::HighPrecisionCounter()
    :   m_started(false)
{

}

//---------------------------------------------------------------------------------------------------------------------
// Resets the counter to the current time.
//---------------------------------------------------------------------------------------------------------------------
void HighPrecisionCounter::ResetCounter()
{
    m_startTime = HighResolutionClock::now();
    m_started = true;
}

//---------------------------------------------------------------------------------------------------------------------
// Returns the number of seconds since the counter was last reset.
//---------------------------------------------------------------------------------------------------------------------
double HighPrecisionCounter::GetDeltaFromStart()
{
    TimePoint endTime = HighResolutionClock::now();
    duration<double> timeSpan = duration_cast<duration<double>>(endTime - m_startTime);
    return timeSpan.count();
}

bool HighPrecisionCounter::Started()
{
    return m_started;
}

}  // end namespace Framework
