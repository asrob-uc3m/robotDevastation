#include "WindowEvent.hpp"

rd::RdWindowEvent::RdWindowEvent(int eventId) : event_id(eventId)
{
}

int rd::RdWindowEvent::getEvent() const
{
    return event_id;
}

const int rd::RdWindowEvent::WINDOW_UNKNOWN = -1;
const int rd::RdWindowEvent::WINDOW_CLOSE = 1;
