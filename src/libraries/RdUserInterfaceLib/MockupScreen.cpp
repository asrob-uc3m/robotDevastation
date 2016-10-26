#include "MockupScreen.hpp"

//-- Public
const std::string rd::MockupScreen::PARAM_MESSAGE = "message";

//-- Protected
const std::string rd::MockupScreen::IMAGE_PATH = "../images/unittesting.jpg";
const std::string rd::MockupScreen::FONT_PATH = "../fonts/FreeMono.ttf";

//-- Private
const SDL_Color rd::MockupScreen::TEXT_COLOR = {255,0,0,0};

rd::MockupScreen::MockupScreen()
{
    w = 200; h = 100; //-- Arbitrary size initialization
    screen = NULL;
}

bool rd::MockupScreen::init()
{
    //-- Find the real path to the resources with ResourceFinder
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load unit test image resource
    background = IMG_Load(rf.findFileByName(IMAGE_PATH).c_str());
    if (background == NULL)
    {
        RD_ERROR("Unable to load background image (resource: %s)!\n SDL_image Error: %s\n", IMAGE_PATH.c_str(), IMG_GetError())
        return false;
    }

    //-- Load the font
    font = TTF_OpenFont(rf.findFileByName(FONT_PATH).c_str(), 32);
    if (font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", rf.findFileByName(FONT_PATH).c_str(), TTF_GetError());
        return false;
    }
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);

    window = NULL;

    //-- Default values:
    this->update(PARAM_MESSAGE, "");
    w = background->w;
    h = background->h;

    return true;
}

bool rd::MockupScreen::cleanup()
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
}

bool rd::MockupScreen::show()
{
    if (window == NULL)
    {
        //-- Init screen
        window = SDL_CreateWindow("MockupScreen",
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  background->w,
                                  background->h,
                                  0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
        if (!window)
        {
            RD_ERROR("Unable to set video mode: %s\n", SDL_GetError());
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

bool rd::MockupScreen::drawScreen(void *screen)
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

rd::MockupScreen::~MockupScreen()
{

}

bool rd::MockupScreen::update(std::string parameter, std::string value)
{
    if (parameter == PARAM_MESSAGE)
    {
        //-- Create new text from font
        text_surface = TTF_RenderText_Solid(font, value.c_str(), TEXT_COLOR);
        return true;
    }

    RD_ERROR("No string parameter %s exists.\n", parameter.c_str());
    return false;
}
