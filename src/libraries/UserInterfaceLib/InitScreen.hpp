// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INIT_SCREEN_HPP__
#define __RD_INIT_SCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <yarp/os/ResourceFinder.h>

#include "Screen.hpp"
#include "Utils.hpp"

namespace rd {

class InitScreen : public Screen
{
    public:
        InitScreen();
        virtual bool init();
        virtual bool cleanup();
        virtual bool drawScreen(void *screen);
        virtual ~InitScreen();

    protected:
        static const std::string SPLASH_PATH;
        static const std::string FONT_PATH;

    private:
        TTF_Font * font;
        SDL_Surface * text_surface;
        SDL_Surface * image;
};

}

#endif // __RD_INIT_SCREEN_HPP__
