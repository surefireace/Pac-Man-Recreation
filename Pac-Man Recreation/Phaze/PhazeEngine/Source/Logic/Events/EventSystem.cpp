#include "EventSystem.h"
#include "../../Utils/PhazeLogger.h"

// static defines
std::map<Event::Type, EventHandlers> EventSystem::m_HandlerMap;

// TODO: Make this configurable
const std::string kLogOutputFile = "Logs/EventSysLog.txt";

void EventSystem::SubscribeHandler(const Event::Type& eventType, IEventHandler* pHandler)
{
    // map iterator
    auto mapIter = m_HandlerMap.find(eventType);

    // sanity check -> end of map
    if (mapIter == m_HandlerMap.end())
    {
        m_HandlerMap[eventType].emplace_back(pHandler);
        return;
    }
    
    // iterator for handler
    auto handlerIter = mapIter->second.begin();

    // iterate until we find the handler
    while (handlerIter != mapIter->second.end() && *handlerIter != pHandler)
        ++handlerIter;

    // HACK: if iterator is somehow null
    if (handlerIter == mapIter->second.end())
    {
        m_HandlerMap[eventType].emplace_back(pHandler);
        return;
    }

    // sanity check -> handler already added for this event
    if (pHandler == *handlerIter)
    {
        PHAZE_LOG(PhazeLogger::LogCategory::kCritical, kLogOutputFile, "handler already added!");
    }
    else
    {
        m_HandlerMap[eventType].emplace_back(pHandler);
    }
    
}

void EventSystem::UnsubscribeHandler(const Event::Type& eventType, IEventHandler* pHandler)
{
    // map iterator
    auto mapIter = m_HandlerMap.find(eventType);

    // sanity check -> end of map
    if (mapIter != m_HandlerMap.end())
    {
        // iterator for handler
        auto handlerIter = mapIter->second.begin();

        // iterate until we find the handler
        while (handlerIter != mapIter->second.end() && *handlerIter != pHandler)
            ++handlerIter;

        // remove the handler
        mapIter->second.erase(handlerIter);
    }
}

void EventSystem::BroadcastEvent(Event* pEvent)
{
    // map iterator
    auto mapIter = m_HandlerMap.find(pEvent->m_type);

    // sanity check -> end of map
    if (mapIter != m_HandlerMap.end())
    {
        // broadcast to each handler
        for (IEventHandler* pHandler : mapIter->second)
            pHandler->HandleEvent(pEvent);
    }
}
