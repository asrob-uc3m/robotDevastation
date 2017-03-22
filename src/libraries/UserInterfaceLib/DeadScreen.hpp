// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __DEAD_SCREEN_HPP__
#define __DEAD_SCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <yarp/os/ResourceFinder.h>

#include "Screen.hpp"

#include "Utils.hpp"
#include "SDLUtils.hpp"

namespace rd {

class DeadScreen : public RdScreen
{
    public:
        DeadScreen();
        virtual bool init();
        virtual bool cleanup();
        virtual bool drawScreen(void *screen);
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
        TTF_Font * font;
        SDL_Surface * text_surface;
        SDL_Surface * skull_image;
        SDL_Surface * camera_frame;
        static const SDL_Color TEXT_COLOR;

        std::string remaining_time;
        RdImage last_camera_frame;
};

}

#endif // __DEAD_SCREEN_HPP__
