// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_EVENT_FACTORY__
#define __RD_SDL_EVENT_FACTORY__


#include "Key.hpp"
#include "WindowEvent.hpp"
#include <map>
#include <SDL.h>


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Factory class that provides static methods for instantiating SDL event source classes.
 *
 */
class RdSDLEventFactory
{
    public:
        //! @brief Creates a \ref RdKey from a SDL keycode
        static rd::RdKey makeKey(SDL_Keycode keycode);

        //! @brief Creates a \ref RdWindowEvent from a SDL window event structure
        static rd::RdWindowEvent makeWindowEvent(SDL_WindowEvent windowEvent);

        //! @brief Initialize the lookup tables
        static bool initLookupTables();
    private:
        RdSDLEventFactory();
        RdSDLEventFactory(const RdSDLEventFactory &);

        //! @brief Boolean indicating that the lookup tables have been initialized or not
        static bool initialized;

        //! @brief Map storing the correspondence between SDLKeys and their corresponding char respresentation
        static std::map< SDL_Keycode, char> sdl_printable_map;

        //! @brief Map storing the correspondence between SDL_Keys and their corresponding control key value
        static std::map< SDL_Keycode, int> sdl_control_map;
};

}

#endif //-- __RD_SDL_EVENT_FACTORY__
