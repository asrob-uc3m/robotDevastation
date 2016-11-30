#include "RdSDLWindowEvent.hpp"

rd::RdSDLWindowEvent::RdSDLWindowEvent(const SDL_WindowEvent & windowEvent)
{
    event_id = windowEvent.event;
}

rd::RdSDLWindowEvent::~RdSDLWindowEvent()
{
}
