// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "MockScreen.hpp"

#include <SDL_image.h>

#include <yarp/os/LogStream.h>
#include <yarp/os/ResourceFinder.h>

#include "LogComponent.hpp"

//-- Public
const std::string rd::MockScreen::PARAM_MESSAGE = "message";

//-- Protected
const std::string rd::MockScreen::IMAGE_PATH = "../images/unittesting.jpg";
const std::string rd::MockScreen::FONT_PATH = "../fonts/FreeMono.ttf";

//-- Private
const SDL_Color rd::MockScreen::TEXT_COLOR = {255,0,0,0};

rd::MockScreen::MockScreen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
    window = NULL;
    screen = NULL;
    font = NULL;
    text_surface = NULL;
    background = NULL;
}

bool rd::MockScreen::init()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load unit test image resource
    background = IMG_Load(rf.findFileByName(IMAGE_PATH).c_str());
    if (background == NULL)
    {
        yCError(RD_UI) << "Background image" << IMAGE_PATH << "failed to load:" << IMG_GetError();
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

    window = NULL;

    //-- Default values:
    update(PARAM_MESSAGE, "");
    w = background->w;
    h = background->h;

    return true;
}

bool rd::MockScreen::cleanup()
{
    if (screen!=NULL)
        SDL_FreeSurface(screen);

    if (background!=NULL)
        SDL_FreeSurface(background);

    if (text_surface!=NULL)
        SDL_FreeSurface(text_surface);

    if (window!=NULL)
        SDL_DestroyWindow(window);

    screen = NULL;
    window = NULL;
    background = NULL;
    text_surface = NULL;
    return true;
}

bool rd::MockScreen::show()
{
    if (window == NULL)
    {
        //-- Init screen
        window = SDL_CreateWindow("MockScreen",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  background->w,
                                  background->h,
                                  0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
        if (!window)
        {
            yCError(RD_UI) << "Unable to set video mode:" << SDL_GetError();
            return false;
        }

        //Get window surface
        screen = SDL_GetWindowSurface( window );
    }

    //-- Clear screen
    SDL_FillRect(screen, NULL, 0xFFFFFF);


    //-- Draw background
    SDL_Rect background_rect = {0,0, background->w, background->h};
    SDL_BlitSurface(background, NULL, screen, &background_rect);

    //-- Draw text
    SDL_Rect text_rect = {(background->w-text_surface->w)/2,(background->h-text_surface->h)/2, text_surface->w, text_surface->h};
    SDL_BlitSurface(text_surface, NULL, screen, &text_rect);


    SDL_UpdateWindowSurface(window); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)

    return true;
}

bool rd::MockScreen::drawScreen(void *screen)
{
    SDL_Surface * sdl_screen = (SDL_Surface *)screen;

    //-- Clear screen
    SDL_FillRect(sdl_screen, NULL, 0xFFFFFF);


    //-- Draw background
    SDL_Rect background_rect = {0,0, background->w, background->h};
    SDL_BlitSurface(background, NULL, sdl_screen, &background_rect);

    //-- Draw text
    SDL_Rect text_rect = {(background->w-text_surface->w)/2,(background->h-text_surface->h)/2, text_surface->w, text_surface->h};
    SDL_BlitSurface(text_surface, NULL, sdl_screen, &text_rect);

    return true;
}

rd::MockScreen::~MockScreen()
{

}

bool rd::MockScreen::update(const std::string & parameter, const std::string & value)
{
    if (parameter == PARAM_MESSAGE)
    {
        //-- Create new text from font
        text_surface = TTF_RenderText_Solid(font, value.c_str(), TEXT_COLOR);
        return true;
    }

    yCError(RD_UI) << "No string parameter" << parameter << "exists";
    return false;
}
