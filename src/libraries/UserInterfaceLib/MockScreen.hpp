// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCK_SCREEN_HPP__
#define __MOCK_SCREEN_HPP__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <yarp/os/ResourceFinder.h>

#include "Screen.hpp"

#include "Utils.hpp"
#include "SDLUtils.hpp"

namespace rd {

class MockScreen : public Screen
{
    public:
        MockScreen();
        virtual bool init();
        virtual bool cleanup();
        virtual bool show();
        virtual bool drawScreen(void *screen);
        virtual ~MockScreen();
        virtual bool update(std::string parameter, std::string value);

        //-- Available parameters:
        static const std::string PARAM_MESSAGE;

    protected:
        static const std::string IMAGE_PATH;
        static const std::string FONT_PATH;

    private:
        SDL_Window * window;
        SDL_Surface * screen;
        TTF_Font * font;
        SDL_Surface * text_surface;
        SDL_Surface * background;
        static const SDL_Color TEXT_COLOR;

        std::string remaining_time;
};

}

#endif // __MOCK_SCREEN_HPP__
