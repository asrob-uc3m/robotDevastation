#include "SDLScreenManager.hpp"

//-- This is very important:
rd::SDLScreenManager * rd::SDLScreenManager::uniqueInstance = NULL;
const std::string rd::SDLScreenManager::id = "SDL";

rd::SDLScreenManager::SDLScreenManager()
{
    stopped = true;
    sdl_initialized = false;
    window = NULL;
}

bool rd::SDLScreenManager::show()
{
    //-- Check if window exists (and create window if it doesn't)
    if (window == NULL)
    {
        //-- Init screen
        window = SDL_CreateWindow("Robot Devastation",
                                  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  200, 100, //-- Arbitrary initial size
                                  0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
        if (window==NULL)
        {
            RD_ERROR("Unable to set video mode: %s\n", SDL_GetError());
            return false;
        }
    }

    //-- Resize window if required
    int h, w;
    SDL_GetWindowSize(window, &w, &h);
    if (screen->w != w || screen->h != h)
        SDL_SetWindowSize(window, screen->w, screen->h);

    //-- Get writable surface
    SDL_Surface * screen_surface = SDL_GetWindowSurface(window);

    //-- Draw current screen on surface
    if (!screen->drawScreen((void *)screen_surface))
    {
        RD_ERROR("Could not draw screen!\n");
        return false;
    }

    //-- Show current screen
    SDL_UpdateWindowSurface(window); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)

    return true;
}

bool rd::SDLScreenManager::initSDL()
{
    //-- Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        RD_ERROR("SDL could not initialize!\n SDL Error: %s\n", SDL_GetError());
        return false;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    //-- Init ttf
    if (TTF_Init() == -1)
    {
        RD_ERROR("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
        return false;
    }

    //Initialize PNG loading
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        RD_ERROR("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

bool rd::SDLScreenManager::cleanupSDL()
{
    RD_WARNING("SDL cleanup not implemented!\n");
    return true;
}

bool rd::SDLScreenManager::start()
{
    if (!sdl_initialized)
    {
        initSDL();
        sdl_initialized = true;
    }

    stopped = false;
    return true;
}

bool rd::SDLScreenManager::stop()
{
    stopped = true;
    return true;
}

bool rd::SDLScreenManager::isStopped()
{
    return stopped;
}

bool rd::SDLScreenManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new SDLScreenManager();
    }

    return Register( uniqueInstance, id);
}

rd::SDLScreenManager::~SDLScreenManager()
{
    if (sdl_initialized)
    {
        cleanupSDL();
        sdl_initialized = false;
    }

    //-- Stop this manager
    this->stop();
    uniqueInstance = NULL;
}
