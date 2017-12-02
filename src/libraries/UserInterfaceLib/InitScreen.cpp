// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "InitScreen.hpp"

#include <SDL_image.h>

#include <yarp/os/ResourceFinder.h>
#include <yarp/os/ConstString.h>

#include "Macros.hpp"

const std::string rd::InitScreen::SPLASH_PATH = "../images/800px-Devastation-thin.png";
const std::string rd::InitScreen::FONT_PATH = "../fonts/FreeMono.ttf";

rd::InitScreen::InitScreen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
    font = NULL;
    text_surface = NULL;
    image = NULL;
}

bool rd::InitScreen::init()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load splash screen resource
    yarp::os::ConstString splashPath = rf.findFileByName(SPLASH_PATH);
    if (splashPath == "")
    {
        RD_ERROR("Unable to find splash screen (resource: %s)!\n", splashPath.c_str());
        return false;
    }
    image = IMG_Load(splashPath.c_str());
    if (image == NULL)
    {
        RD_ERROR("Unable to load splash screen (resource: %s)!\n SDL_image Error: %s\n", splashPath.c_str(), IMG_GetError());
        return false;
    }

    //-- Load the font
    font = TTF_OpenFont(rf.findFileByName(FONT_PATH).c_str(), 28);
    if (font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", rf.findFileByName(FONT_PATH).c_str(), TTF_GetError());
        return false;
    }

    //-- Create text from font
    SDL_Color text_color = {0,255,0,0};
    text_surface = TTF_RenderText_Solid(font, "Press any key to start", text_color);

    //-- Default values:
    w = image->w;
    h = image->h;

    return true;
}

bool rd::InitScreen::cleanup()
{
    if (image!=NULL)
        SDL_FreeSurface(image);

    if (text_surface!=NULL)
        SDL_FreeSurface(text_surface);

    image = NULL;
    text_surface = NULL;
    return true;
}

bool rd::InitScreen::drawScreen(void *screen)
{
    SDL_Surface * sdl_screen = (SDL_Surface *)screen;

    //-- Clear screen
    SDL_FillRect(sdl_screen, NULL, 0x000000);

    //-- Draw splash
    SDL_Rect splash_rect = {0,0, image->w, image->h};
    SDL_BlitSurface(image, NULL, sdl_screen, &splash_rect);

    //-- Draw text
    SDL_Rect text_rect = {(image->w-text_surface->w)/2,image->h, text_surface->w, text_surface->h};
    SDL_BlitSurface(text_surface, NULL, sdl_screen, &text_rect);

    return true;
}

rd::InitScreen::~InitScreen()
{
}
