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
        char getChar();
        bool setFromKeyCode(SDLKey keycode);

    private:
        SDLKey key_value;
        static const std::map< SDLKey, char> sdl_map;
};

}

#endif //-- __RD_KEY_HPP__
