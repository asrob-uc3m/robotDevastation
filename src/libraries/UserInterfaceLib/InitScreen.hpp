// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_INIT_SCREEN_HPP__
#define __RD_INIT_SCREEN_HPP__

#include <string>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Screen.hpp"

namespace rd {

/**
 * @ingroup UserInterfaceLib
 *
 * @brief A User Interface.
 */
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
