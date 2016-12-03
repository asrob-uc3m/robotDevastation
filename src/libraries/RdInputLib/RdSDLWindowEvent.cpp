#include "RdSDLWindowEvent.hpp"

rd::RdSDLWindowEvent::RdSDLWindowEvent(const SDL_WindowEvent & windowEvent)
{
    identifyAction(windowEvent.event);
}

rd::RdSDLWindowEvent::~RdSDLWindowEvent()
{
}

void rd::RdSDLWindowEvent::identifyAction(int id)
{
    switch (id)
    {
    case SDL_WINDOWEVENT_CLOSE: event_id = WINDOW_CLOSE;   return;
    default:                    event_id = WINDOW_UNKNOWN; return;
    }
}
