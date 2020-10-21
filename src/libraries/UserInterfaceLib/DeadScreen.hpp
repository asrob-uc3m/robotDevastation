// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_DEAD_SCREEN_HPP__
#define __RD_DEAD_SCREEN_HPP__

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Screen.hpp"
#include "ImageManager.hpp"

namespace rd {

/**
 * @ingroup UserInterfaceLib
 *
 * @brief A User Interface.
 */
class DeadScreen : public Screen
{
    public:
        DeadScreen();
        virtual bool init();
        virtual bool cleanup();
        virtual bool drawScreen(void *screen);
        virtual ~DeadScreen();
        virtual bool update(const std::string & parameter, const std::string & value);
        virtual bool update(const std::string & parameter, const Image & value);

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
        Image last_camera_frame;
};

}

#endif // __RD_DEAD_SCREEN_HPP__
