// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SDL_KEY_HPP__
#define __RD_SDL_KEY_HPP__


#include "RdKey.hpp"
#include <map>
#include <SDL.h>


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Class that represents a keyboard key, implemented with SDL
 *
 * This key is created from a SDL keycode, usually obtained by the SDL event
 *
 */
class RdSDLKey
{
    public:
        //! @brief Creates a RdKey from a SDL keycode
        static RdKey makeKey(SDL_Keycode keycode);

        //! @brief Initialize the lookup tables
        static bool initLookupTables();
    private:
        RdSDLKey();

        //! @brief Boolean indicating that the lookup tables have been initialized or not
        static bool initialized;

        //! @brief Map storing the correspondence between SDLKeys and their corresponding char respresentation
        static std::map< SDL_Keycode, char> sdl_printable_map;
        //! @brief Map storing the correspondence between SDL_Keys and their corresponding control key value
        static std::map< SDL_Keycode, int> sdl_control_map;
};

}

#endif //-- __RD_KEY_HPP__
