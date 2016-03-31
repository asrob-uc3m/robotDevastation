#include "DeadScreen.hpp"

//-- Public
const std::string rd::DeadScreen::PARAM_REMAINING_TIME = "remaining_time";
const std::string rd::DeadScreen::PARAM_LAST_CAMERA_FRAME = "last_camera_frame";

//-- Protected
const std::string rd::DeadScreen::SKULL_PATH = "../../share/images/skull.png";
const std::string rd::DeadScreen::FONT_PATH = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";

//-- Private
const SDL_Color rd::DeadScreen::TEXT_COLOR = {0,255,0,0};

rd::DeadScreen::DeadScreen()
{
    //-- Default values:
    this->update(PARAM_REMAINING_TIME, "10");

    //-- Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        RD_ERROR("SDL could not initialize!\n SDL Error: %s\n", SDL_GetError());
        return;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    //-- Init ttf
    if (TTF_Init() == -1)
    {
      RD_ERROR("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
      return;
    }

    //Initialize PNG loading
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        RD_ERROR("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return;
    }

    //-- Load splash screen resource
    image = IMG_Load(SKULL_PATH.c_str());
    if (image == NULL)
    {
        RD_ERROR("Unable to load skull image (resource: %s)!\n SDL_image Error: %s\n", SKULL_PATH.c_str(), IMG_GetError())
        return;
    }

    //-- Load the font
    font = TTF_OpenFont(FONT_PATH.c_str(), 28);
    if (font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", FONT_PATH.c_str(), TTF_GetError());
        return;
    }

    //-- Screen surface
    screen = SDL_SetVideoMode(image->w, image->h, 16, SDL_DOUBLEBUF);
    if (!screen)
    {
        RD_ERROR("Unable to set video mode: %s\n", SDL_GetError());
        return;
    }
}

bool rd::DeadScreen::show()
{
    //-- Clear screen
    SDL_FillRect(screen, NULL, 0xFFFFFF);

    //-- Draw splash
    SDL_Rect splash_rect = {0,0, image->w, image->h};
    SDL_BlitSurface(image, NULL, screen, &splash_rect);

    //-- Draw text    
    SDL_Rect text_rect = {(image->w-text_surface->w)/2,image->h-text_surface->h, text_surface->w, text_surface->h};
    SDL_BlitSurface(text_surface, NULL, screen, &text_rect);

    SDL_Flip(screen); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)
}

rd::DeadScreen::~DeadScreen()
{
    SDL_FreeSurface(screen);
    SDL_FreeSurface(image);
    SDL_FreeSurface(text_surface);
}

bool rd::DeadScreen::update(std::string parameter, std::string value)
{
    if (parameter == PARAM_REMAINING_TIME)
    {
        //-- Create new text from font
        remaining_time = value;
        text_surface = TTF_RenderText_Solid(font, ("Respawn in: "+remaining_time+"s").c_str(), TEXT_COLOR);
    }

    return true;
}

bool rd::DeadScreen::update(std::string parameter, rd::RdImage value)
{
 return false;
}
