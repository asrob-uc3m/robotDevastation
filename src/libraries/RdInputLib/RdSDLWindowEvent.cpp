#include "RdSDLWindowEvent.hpp"

rd::RdWindowEvent rd::RdSDLWindowEvent::makeWindowEvent(const SDL_WindowEvent & windowEvent)
{
    switch ( windowEvent.event )
    {
    case SDL_WINDOWEVENT_CLOSE: return rd::RdWindowEvent::WINDOW_CLOSE;
    default:                    return rd::RdWindowEvent::WINDOW_UNKNOWN;
    }
}
