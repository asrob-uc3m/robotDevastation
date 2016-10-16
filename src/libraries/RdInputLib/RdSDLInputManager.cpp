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

rd::RdSDLInputManager::RdSDLInputManager()
{
    stopped = true;

    //-- Init SDL
    ////////if (SDL_WasInit( SDL_INIT_EVENTTHREAD) == 0)
    ////////////7    SDL_Init(SDL_INIT_EVENTTHREAD);

    //-- Init X11 threads
    XInitThreads();
}

void rd::RdSDLInputManager::inputCallback(SDL_Event *event)
{
    if (event->type == SDL_KEYDOWN )
    {
        RD_DEBUG("Event: Keydown\n");
    }
    else if (event->type == SDL_KEYUP )
    {
        RD_DEBUG("Event: Keyup\n");
    }
    else
    {
        RD_DEBUG("Event: Unknown\n");
    }

    //-- Check for event
    /*RdKey * key = NULL;
    SDL_Event event;

    while( SDL_PollEvent( &event ) )
    {
        if (event.type == SDL_KEYDOWN )
        {
            key = new RdSDLKey(event.key.keysym.sym);

            if ( !(key->isPrintable() || key->isControlKey()) )
            {
                delete key;
                key = NULL;
                //return false;
                return;
            }

            for ( int i = 0; i < (int)listeners.size(); i++)
                listeners.at(i)->onKeyDown(*key);

        }
        else if (event.type == SDL_KEYUP )
        {
             key = new RdSDLKey(event.key.keysym.sym);

             if ( !(key->isPrintable() || key->isControlKey()) )
             {
                 delete key;
                 key = NULL;
                 //return false;
                 return;
             }

            for ( int i = 0; i < (int)listeners.size(); i++)
                listeners.at(i)->onKeyUp(*key);

        }
        else
        {
//          RD_WARNING("Unkown event ocurred! (Event is not supported yet)\n");
            //return false;
            return;
        }
    }

    delete key;
    key = NULL;
    //return true;*/
    return;
}
