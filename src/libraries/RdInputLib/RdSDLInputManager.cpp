#include "RdSDLInputManager.hpp"

//-- Initialize static members
rd::RdSDLInputManager * rd::RdSDLInputManager::uniqueInstance = NULL;
const std::string rd::RdSDLInputManager::id = "SDL";

bool rd::RdSDLInputManager::start()
{  
    //-- Start input callback
    SDL_AddEventWatch(staticInputCallback, this);

    stopped = false;

    return true;
}

bool rd::RdSDLInputManager::stop()
{
    //-- Stop input callback
    SDL_DelEventWatch(staticInputCallback, this);

    stopped = true;

    return true;
}

bool rd::RdSDLInputManager::isStopped()
{
    return stopped;
}

bool rd::RdSDLInputManager::configure(std::string parameter, std::string value)
{
    return RdInputManager::configure(parameter, value);
}

bool rd::RdSDLInputManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdSDLInputManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdSDLInputManager::~RdSDLInputManager()
{
    //-- Stop this thread
    this->stop();
    uniqueInstance = NULL;
}

bool rd::RdSDLInputManager::refreshEvents()
{
    SDL_PumpEvents();
    return true;
}

rd::RdSDLInputManager::RdSDLInputManager()
{
    stopped = true;

    //-- Init SDL
    ////////if (SDL_WasInit( SDL_INIT_EVENTTHREAD) == 0)
    ////////////7    SDL_Init(SDL_INIT_EVENTTHREAD);

    //-- Init X11 threads
    XInitThreads();
}

bool rd::RdSDLInputManager::inputCallback(SDL_Event *event)
{
    RdKey * key = NULL;

    if (event->type == SDL_KEYDOWN )
    {
        key = new RdSDLKey(event->key.keysym.sym);

        if ( !(key->isPrintable() || key->isControlKey()) )
        {
            delete key;
            key = NULL;
            return false;
        }

        for ( int i = 0; i < (int)listeners.size(); i++)
            listeners.at(i)->onKeyDown(*key);

    }
    else if (event->type == SDL_KEYUP )
    {
         key = new RdSDLKey(event->key.keysym.sym);

         if ( !(key->isPrintable() || key->isControlKey()) )
         {
             delete key;
             key = NULL;
             return false;
         }

        for ( int i = 0; i < (int)listeners.size(); i++)
            listeners.at(i)->onKeyUp(*key);

    }
    else
    {
//          RD_WARNING("Unkown event ocurred! (Event is not supported yet)\n");
        return false;
    }

    delete key;
    key = NULL;
    return true;

}
