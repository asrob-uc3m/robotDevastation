// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __DEAD_SCREEN_HPP__
#define __DEAD_SCREEN_HPP__

#include <SDL/SDL.h>
#include <SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cstdlib> // For some useful functions such as atexit :)
#include <string>

#include "RdScreen.hpp"

#include "RdUtils.hpp"

namespace rd {

class DeadScreen : public RdScreen
{
    public:
        DeadScreen();
        virtual bool show();
        virtual ~DeadScreen();
        virtual bool update(std::string parameter, std::string value);
        virtual bool update(std::string parameter, RdImage value);

        //-- Available parameters:
        static const std::string PARAM_REMAINING_TIME;
        static const std::string PARAM_LAST_CAMERA_FRAME;

    protected:
        static const std::string SKULL_PATH;
        static const std::string FONT_PATH;

    private:
        SDL_Surface * screen;
        TTF_Font * font;
        SDL_Surface * text_surface;
        SDL_Surface * image;

        std::string remaining_time;
        RdImage last_camera_frame;
};

}

#endif // __DEAD_SCREEN_HPP__
