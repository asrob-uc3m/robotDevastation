#include "RdWindowEvent.hpp"

rd::RdWindowEvent::RdWindowEvent() : event_id(-1)
{
}

int rd::RdWindowEvent::getEvent() const
{
    return event_id;
}

rd::RdWindowEvent::~RdWindowEvent()
{
}

const int rd::RdWindowEvent::WINDOW_CLOSE = 0;
