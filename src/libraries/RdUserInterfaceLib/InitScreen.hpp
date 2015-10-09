// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __INIT_SCREEN_HPP__
#define __INIT_SCREEN_HPP__

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cstdlib> // For some useful functions such as atexit :)
#include <string>

#include "RdScreen.hpp"
#include "RdUtils.hpp"

namespace rd {

class InitScreen : public RdScreen
{
    public:
        InitScreen();
        virtual bool show();
        virtual ~InitScreen();

    protected:
        static const std::string SPLASH_PATH;
        static const std::string FONT_PATH;

    private:
        SDL_Surface * screen;
        TTF_Font * font;
        SDL_Surface * text_surface;
        SDL_Surface * image;
};

}

#endif // __INIT_SCREEN_HPP__