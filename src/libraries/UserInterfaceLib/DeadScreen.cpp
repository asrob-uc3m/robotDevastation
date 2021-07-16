// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "DeadScreen.hpp"

#include <SDL_image.h>

#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>

#include "LogComponent.hpp"
#include "SDLUtils.hpp"

//-- Public
const std::string rd::DeadScreen::PARAM_REMAINING_TIME = "remaining_time";
const std::string rd::DeadScreen::PARAM_LAST_CAMERA_FRAME = "last_camera_frame";

//-- Protected
const std::string rd::DeadScreen::SKULL_PATH = "../images/skull.png";
const std::string rd::DeadScreen::FONT_PATH = "../fonts/FreeMono.ttf";

//-- Private
const SDL_Color rd::DeadScreen::TEXT_COLOR = {0,255,0,0};

rd::DeadScreen::DeadScreen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
    font = NULL;
    text_surface = NULL;
    skull_image = NULL;
    camera_frame = NULL;
}

bool rd::DeadScreen::init()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load skull image resource
    skull_image = IMG_Load(rf.findFileByName(SKULL_PATH).c_str());
    if (skull_image == NULL)
    {
        yCError(RD_UI) << "Skull image" << SKULL_PATH << "failed to load:" << IMG_GetError();
        return false;
    }

    //-- Load the font
    font = TTF_OpenFont(rf.findFileByName(FONT_PATH).c_str(), 32);
    if (font == NULL)
    {
        yCError(RD_UI) << "Font" << rf.findFileByName(FONT_PATH) << "failed to load:" << TTF_GetError();
        return false;
    }
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    //-- Default values:
    w = skull_image->w;
    h = skull_image->h;
    camera_frame = NULL;
    update(PARAM_REMAINING_TIME, "10");

    return true;
}

bool rd::DeadScreen::cleanup()
{
    if (skull_image!=NULL)
        SDL_FreeSurface(skull_image);
    if (text_surface!=NULL)
    SDL_FreeSurface(text_surface);
    if (camera_frame!=NULL)
        SDL_FreeSurface(camera_frame);

    skull_image = NULL;
    camera_frame = NULL;

    return true;
}

bool rd::DeadScreen::drawScreen(void *screen)
{
    SDL_Surface * sdl_screen = (SDL_Surface *)screen;

    if (camera_frame)
    {
        //-- Draw camera frame
        SDL_Rect camera_frame_rect = {0,0, camera_frame->w, camera_frame->h};
        SDL_BlitSurface(camera_frame, NULL, sdl_screen, &camera_frame_rect);

        //-- Draw skull
        SDL_Rect skull_rect = {(camera_frame->w-skull_image->w)/2,(camera_frame->h-skull_image->h)/2,
                               skull_image->w, skull_image->h};
        SDL_BlitSurface(skull_image, NULL, sdl_screen, &skull_rect);

        //-- Draw text
        SDL_Rect text_rect = {(camera_frame->w-text_surface->w)/2,camera_frame->h-text_surface->h,
                              text_surface->w, text_surface->h};
        SDL_BlitSurface(text_surface, NULL, sdl_screen, &text_rect);
    }
    else
    {
        //-- Draw skull
        SDL_Rect skull_rect = {0,0, skull_image->w, skull_image->h};
        SDL_BlitSurface(skull_image, NULL, sdl_screen, &skull_rect);

        //-- Draw text
        SDL_Rect text_rect = {(skull_image->w-text_surface->w)/2,skull_image->h-text_surface->h, text_surface->w, text_surface->h};
        SDL_BlitSurface(text_surface, NULL, sdl_screen, &text_rect);
    }

    return true;
}

rd::DeadScreen::~DeadScreen()
{

}

bool rd::DeadScreen::update(const std::string & parameter, const std::string & value)
{
    if (parameter == PARAM_REMAINING_TIME)
    {
        //-- Create new text from font
        remaining_time = value;
        text_surface = TTF_RenderText_Solid(font, ("Respawn in: "+remaining_time+"s").c_str(), TEXT_COLOR);
        return true;
    }

    yCError(RD_UI) << "No string parameter" << parameter << "exists";
    return false;
}

bool rd::DeadScreen::update(const std::string & parameter, const Image & value)
{
    if (value.width() == 0 || value.height() == 0)
    {
        yCError(RD_UI) << "Invalid image";
        return false;
    }

    if (parameter == PARAM_LAST_CAMERA_FRAME)
    {
        last_camera_frame = Image(value);

        //-- Convert from Image to SDL
        camera_frame = RdImage2SDLImage(last_camera_frame);

        //-- Set new window size
        w = camera_frame->w;
        h = camera_frame->h;

        return true;
    }

    yCError(RD_UI) << "No image parameter" << parameter << "exists";
    return false;
}
