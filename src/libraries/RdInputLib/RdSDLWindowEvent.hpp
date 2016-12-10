// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_WINDOW_EVENT__
#define __RD_SDL_WINDOW_EVENT__


#include "RdWindowEvent.hpp"
#include <SDL.h>


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Class that represents a window event, implemented with SDL
 *
 */
class RdSDLWindowEvent
{
    public:
        static rd::RdWindowEvent makeWindowEvent(const SDL_WindowEvent & windowEvent);

    private:
        RdSDLWindowEvent();
};

}

#endif //-- __RD_SDL_WINDOW_EVENT__
