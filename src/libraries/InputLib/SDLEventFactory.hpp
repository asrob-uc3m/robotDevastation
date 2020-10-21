// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_SDL_EVENT_FACTORY__
#define __RD_SDL_EVENT_FACTORY__

#include <map>

#include <SDL.h>

#include "Key.hpp"
#include "WindowEvent.hpp"

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
        //! @brief Creates a Key from a SDL keycode
        static Key makeKey(SDL_Keycode keycode);

        //! @brief Creates a WindowEvent from a SDL window event structure
        static WindowEvent makeWindowEvent(SDL_WindowEvent windowEvent);

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
