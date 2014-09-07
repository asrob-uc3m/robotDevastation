// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_KEY_HPP__
#define __RD_KEY_HPP__

#include "RdUtils.hpp"

#include "SDL/SDL.h"
#include <map>

namespace rd{

class RdKey
{
    public:
        RdKey();
        bool setFromKeyCode(SDLKey keycode);

        bool isControlKey();
        bool isPrintable();

        char getChar();
        int  getValue();

        //-- Constants for control keys (non-printable)
        static const int KEY_UNKNOWN;
        static const int KEY_PRINTABLE;
        static const int KEY_SPACE;
        static const int KEY_ESCAPE;
        static const int KEY_BACKSPACE;
        static const int KEY_ARROW_UP;
        static const int KEY_ARROW_DOWN;
        static const int KEY_ARROW_LEFT;
        static const int KEY_ARROW_RIGHT;

    private:
        char char_value;
        int key_value;

        bool printable;
        bool control;

        static const std::map< SDLKey, char> sdl_printable_map;
        static const std::map< SDLKey, int> sdl_control_map;
};

}

#endif //-- __RD_KEY_HPP__
