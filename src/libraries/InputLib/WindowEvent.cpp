#include "WindowEvent.hpp"

rd::WindowEvent::WindowEvent(int eventId) : event_id(eventId)
{
}

int rd::WindowEvent::getEvent() const
{
    return event_id;
}

const int rd::WindowEvent::WINDOW_UNKNOWN = -1;
const int rd::WindowEvent::WINDOW_CLOSE = 1;
