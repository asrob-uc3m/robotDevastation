#include <SDL_image.h>
#include <SDL_ttf.h>

#include "SDLScreenManager.hpp"
#include "Macros.hpp"

//-- This is very important:
rd::SDLScreenManager * rd::SDLScreenManager::uniqueInstance = NULL;
const std::string rd::SDLScreenManager::id = "SDL";

const std::string rd::SDLScreenManager::PARAM_FULLSCREEN = "fullscreen";

rd::SDLScreenManager::SDLScreenManager()
{
    stopped = true;
    sdl_initialized = false;
    fullscreen = false;
    window = NULL;
}

void rd::SDLScreenManager::setCurrentScreen(Screen *screen)
{
    ScreenManager::setCurrentScreen(screen);
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }
}

bool rd::SDLScreenManager::show()
{
    mutex.lock();

    //-- Check if window exists (and create window if it doesn't)
    if (window == NULL)
    {
        //-- Init screen
        if (fullscreen)
        {
            window = SDL_CreateWindow("Robot Devastation",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      200, 100, //-- Arbitrary initial size
                                      SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
        }
        else
        {
            window = SDL_CreateWindow("Robot Devastation",
                                      SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                      200, 100, //-- Arbitrary initial size
                                      0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
        }
        if (window==NULL)
        {
            RD_ERROR("Unable to set video mode: %s\n", SDL_GetError());
            return false;
        }
    }

    //-- Resize window if required
    int h, w;
    SDL_GetWindowSize(window, &w, &h);
    if (screen->getWidth() != w || screen->getHeight() != h)
        SDL_SetWindowSize(window, screen->getWidth(), screen->getHeight());

    //-- Get writable surface
    SDL_Surface * screen_surface = SDL_GetWindowSurface(window);
    if (screen_surface == NULL)
    {
        RD_ERROR("Could not get SDL screen surface!\n");
        return false;
    }

    //-- Draw current screen on surface
    if (!screen->drawScreen((void *)screen_surface))
    {
        RD_ERROR("Could not draw screen!\n");
        return false;
    }

    //-- Show current screen
    SDL_UpdateWindowSurface(window); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)
    SDL_FreeSurface(screen_surface);

    mutex.unlock();
    return true;
}

bool rd::SDLScreenManager::update(const std::string & parameter, const std::string & value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, Image value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, Player value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, const std::vector<Player> & value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, const std::vector<Target> & value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, Weapon value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::initSDL()
{
    //-- Init SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        RD_ERROR("SDL could not initialize!\n SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //-- Init ttf
    if (TTF_Init() == -1)
    {
        RD_ERROR("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
        cleanupSDL();
        return false;
    }

    //Initialize PNG loading
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        RD_ERROR("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        cleanupSDL();
        return false;
    }
    return true;
}

bool rd::SDLScreenManager::cleanupSDL()
{
    RD_INFO("Freeing resources...\n");
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return true;
}

bool rd::SDLScreenManager::configure(const std::string & parameter, const std::string & value)
{
    if (parameter==PARAM_FULLSCREEN)
    {
        if (value=="enabled")
        {
            fullscreen = true;
            RD_INFO("Fullscreen enabled!\n");
            return true;
        }
        else if (value=="disabled")
        {
            fullscreen = false;
            RD_INFO("Fullscreen disabled!\n");
            return true;
        }
    }

    return ScreenManager::configure(parameter, value);
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
    if (window!=NULL)
        SDL_DestroyWindow(window);
    stopped = true;
    return true;
}

bool rd::SDLScreenManager::isStopped() const
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
    stop();
    uniqueInstance = NULL;
}
