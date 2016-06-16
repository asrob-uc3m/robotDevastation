#include "InitScreen.hpp"

const std::string rd::InitScreen::SPLASH_PATH = "../../share/images/800px-Devastation-thin.png";
const std::string rd::InitScreen::FONT_PATH = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";

rd::InitScreen::InitScreen()
{

}

bool rd::InitScreen::init()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load splash screen resource
    image = IMG_Load(rf.findFileByName(SPLASH_PATH).c_str());
    if (image == NULL)
    {
        RD_ERROR("Unable to load splash screen (resource: %s)!\n SDL_image Error: %s\n", SPLASH_PATH.c_str(), IMG_GetError())
        return false;
    }

    //-- Load the font
    font = TTF_OpenFont(FONT_PATH.c_str(), 28);
    if (font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", FONT_PATH.c_str(), TTF_GetError());
        return false;
    }

    //-- Create text from font
    SDL_Color text_color = {0,255,0,0};
    text_surface = TTF_RenderText_Solid(font, "Press any key to start", text_color);

    screen = NULL;
    return true;
}

bool rd::InitScreen::cleanup()
{
    SDL_FreeSurface(screen);
    SDL_FreeSurface(image);
    SDL_FreeSurface(text_surface);
    screen = NULL;
    image = NULL;
    text_surface = NULL;
    return true;
}

bool rd::InitScreen::show()
{
    if (screen == NULL)
    {
        //-- Init screen
        screen = SDL_SetVideoMode(image->w, image->h+text_surface->h, 16, SDL_DOUBLEBUF);
        if (!screen)
        {
            RD_ERROR("Unable to set video mode: %s\n", SDL_GetError());
            return false;
        }
    }
    //-- Clear screen
    SDL_FillRect(screen, NULL, 0x000000);

    //-- Draw splash
    SDL_Rect splash_rect = {0,0, image->w, image->h};
    SDL_BlitSurface(image, NULL, screen, &splash_rect);

    //-- Draw text
    SDL_Rect text_rect = {(image->w-text_surface->w)/2,image->h, text_surface->w, text_surface->h};
    SDL_BlitSurface(text_surface, NULL, screen, &text_rect);

    SDL_Flip(screen); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)
}

rd::InitScreen::~InitScreen()
{
}
