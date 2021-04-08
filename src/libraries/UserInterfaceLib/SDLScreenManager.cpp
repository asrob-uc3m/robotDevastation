// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "SDLScreenManager.hpp"

#include <SDL_image.h>
#include <SDL_ttf.h>

#include <yarp/os/LogStream.h>

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
            yError() << "Unable to set video mode:" << SDL_GetError();
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
        yError() << "Could not get SDL screen surface!";
        return false;
    }

    //-- Draw current screen on surface
    if (!screen->drawScreen((void *)screen_surface))
    {
        yError() << "Could not draw screen!";
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

bool rd::SDLScreenManager::update(const std::string & parameter, const Image & value)
{
    mutex.lock();
    bool ret = screen->update(parameter, value);
    mutex.unlock();
    return ret;
}

bool rd::SDLScreenManager::update(const std::string & parameter, const Player & value)
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

bool rd::SDLScreenManager::update(const std::string & parameter, const Weapon & value)
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
        yError() << "SDL could not initialize:" << SDL_GetError();
        return false;
    }

    //-- Init ttf
    if (TTF_Init() == -1)
    {
        yError() << "SDL_ttf could not initialize:" << TTF_GetError();
        cleanupSDL();
        return false;
    }

    //Initialize PNG loading
    if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG))
    {
        yError() << "SDL_image could not initialize:" << IMG_GetError();
        cleanupSDL();
        return false;
    }
    return true;
}

bool rd::SDLScreenManager::cleanupSDL()
{
    yInfo() << "Freeing resources...";
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
            yInfo() << "Fullscreen enabled!";
            return true;
        }
        else if (value=="disabled")
        {
            fullscreen = false;
            yInfo() << "Fullscreen disabled!";
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
