// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_KEY_HPP__
#define __RD_KEY_HPP__

#include "RdUtils.hpp"

#include "SDL/SDL.h"
#include <map>

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
class RdKey
{
    public:
        //! @brief Default constructor that creates a dummy key not bound to any real key of a keyboard
        RdKey();
        //! @brief Configures the RdKey internal state from a SDL key code
        bool setFromKeyCode(SDLKey keycode);

        bool isControlKey();
        bool isPrintable();

        char getChar();
        int  getValue();

        //-- Constants for control keys (non-printable)
        //! @brief Constant representing a non-supported / unknown key
        static const int KEY_UNKNOWN;
        //! @brief Constant representing any printable key
        static const int KEY_PRINTABLE;
        static const int KEY_SPACE;
        static const int KEY_ESCAPE;
        static const int KEY_BACKSPACE;
        static const int KEY_ARROW_UP;
        static const int KEY_ARROW_DOWN;
        static const int KEY_ARROW_LEFT;
        static const int KEY_ARROW_RIGHT;

    private:
        //! @brief Stores the char representation of a key
        char char_value;
        //! @brief Stores the control key value of a key
        int key_value;

        //! @brief Stores whether the key is printable or not
        bool printable;
        //! @brief Stores whether the key is a control key or not
        bool control;

        //! @brief Map storing the correspondence between SDLKeys and their corresponding char respresentation
        static const std::map< SDLKey, char> sdl_printable_map;
        //! @brief Map storing the correspondence between SDL_Keys and their corresponding control key value
        static const std::map< SDLKey, int> sdl_control_map;
};

}

#endif //-- __RD_KEY_HPP__
