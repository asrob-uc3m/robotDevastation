#include "RdWindowEvent.hpp"

rd::RdWindowEvent::RdWindowEvent() : event_id(WINDOW_UNKNOWN)
{
}

int rd::RdWindowEvent::getEvent() const
{
    return event_id;
}

rd::RdWindowEvent::~RdWindowEvent()
{
}

const int rd::RdWindowEvent::WINDOW_UNKNOWN = -1;
const int rd::RdWindowEvent::WINDOW_CLOSE = 1;
