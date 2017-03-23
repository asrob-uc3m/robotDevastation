// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_EVENT_FACTORY__
#define __RD_SDL_EVENT_FACTORY__


#include "Key.hpp"
#include "WindowEvent.hpp"
#include <map>
#include <SDL.h>


namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Factory class that provides static methods for instantiating SDL event source classes.
 *
 */
class SDLEventFactory
{
    public:
        //! @brief Creates a \ref Key from a SDL keycode
        static rd::Key makeKey(SDL_Keycode keycode);

        //! @brief Creates a \ref WindowEvent from a SDL window event structure
        static rd::WindowEvent makeWindowEvent(SDL_WindowEvent windowEvent);

        //! @brief Initialize the lookup tables
        static bool initLookupTables();
    private:
        SDLEventFactory();
        SDLEventFactory(const SDLEventFactory &);

        //! @brief Boolean indicating that the lookup tables have been initialized or not
        static bool initialized;

        //! @brief Map storing the correspondence between SDLKeys and their corresponding char respresentation
        static std::map< SDL_Keycode, char> sdl_printable_map;

        //! @brief Map storing the correspondence between SDL_Keys and their corresponding control key value
        static std::map< SDL_Keycode, int> sdl_control_map;
};

}

#endif //-- __RD_SDL_EVENT_FACTORY__
