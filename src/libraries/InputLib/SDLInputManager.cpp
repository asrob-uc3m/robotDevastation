#include "SDLInputManager.hpp"
#include "SDLEventFactory.hpp"
#include "Key.hpp"
#include "WindowEvent.hpp"

//-- Initialize static members
rd::SDLInputManager * rd::SDLInputManager::uniqueInstance = NULL;
const std::string rd::SDLInputManager::id = "SDL";

bool rd::SDLInputManager::start()
{  
    //-- Start input callback
    SDL_AddEventWatch(staticInputCallback, this);

    stopped = false;

    return true;
}

bool rd::SDLInputManager::stop()
{
    //-- Stop input callback
    SDL_DelEventWatch(staticInputCallback, this);

    stopped = true;

    return true;
}

bool rd::SDLInputManager::isStopped() const
{
    return stopped;
}

bool rd::SDLInputManager::configure(const std::string & parameter, const std::string & value)
{
    return InputManager::configure(parameter, value);
}

bool rd::SDLInputManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new SDLInputManager();
    }

    return Register( uniqueInstance, id);
}

rd::SDLInputManager::~SDLInputManager()
{
    //-- Stop this thread
    this->stop();
    uniqueInstance = NULL;
}

bool rd::SDLInputManager::refreshEvents()
{
    SDL_PumpEvents();
    return true;
}

rd::SDLInputManager::SDLInputManager()
{
    stopped = true;

    //-- Init SDL
    ////////if (SDL_WasInit( SDL_INIT_EVENTTHREAD) == 0)
    ////////////7    SDL_Init(SDL_INIT_EVENTTHREAD);
}

bool rd::SDLInputManager::inputCallback(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN )
    {
        Key key = SDLEventFactory::makeKey(event->key.keysym.sym);

        if ( !(key.isPrintable() || key.isControlKey()) )
            return false;

        for ( int i = 0; i < (int)listeners.size(); i++)
            listeners.at(i)->onKeyDown(key);
    }
    else if (event->type == SDL_KEYUP )
    {
        Key key = SDLEventFactory::makeKey(event->key.keysym.sym);

        if ( !(key.isPrintable() || key.isControlKey()) )
            return false;

        for ( int i = 0; i < (int)listeners.size(); i++)
            listeners.at(i)->onKeyUp(key);
    }
    else if (event->type == SDL_WINDOWEVENT)
    {
        WindowEvent windowEvent = SDLEventFactory::makeWindowEvent(event->window);

        for (int i = 0; i < (int) listeners.size(); i++)
            listeners.at(i)->onWindowEvent(windowEvent);
    }
    else
    {
//          RD_WARNING("Unkown event ocurred! (Event is not supported yet)\n");
        return false;
    }

    return true;
}
