// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_KEY_HPP__
#define __RD_SDL_KEY_HPP__


#include "RdKey.hpp"
#include <map>
#include "SDL/SDL.h"


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Class that represents a keyboard key
 *
 * Keys are classified in two groups: printable keys, which are keys that have a char representation,
 * such as the 'a' key or the '7' key, and control keys, which are keys that perform an action, such
 * as the arrow keys, the escape key or the backspace key. The space case is a special case, because
 * it is considered both printable and control key.
 *
 *
 */
class RdSDLKey: public RdKey
{
    public:
        //! @brief Creates a RdKey from a SDL keycode
        RdSDLKey(SDLKey keycode);

        //! @brief Initialize the lookup tables
        static bool initLookupTables();

    private:
        //! @brief Boolean indicating that the lookup tables have been initialized or not
        static bool initialized;

        //! @brief Map storing the correspondence between SDLKeys and their corresponding char respresentation
        static std::map< SDLKey, char> sdl_printable_map;
        //! @brief Map storing the correspondence between SDL_Keys and their corresponding control key value
        static std::map< SDLKey, int> sdl_control_map;
};

}

#endif //-- __RD_KEY_HPP__
