#include "RdSDLInputManager.hpp"

//-- Initialize static members
rd::RdSDLInputManager * rd::RdSDLInputManager::uniqueInstance = NULL;
const std::string rd::RdSDLInputManager::id = "SDL";
const int rd::RdSDLInputManager::UPDATE_RATE_MS = 20;

bool rd::RdSDLInputManager::start()
{  
    //-- Start input thread
    return yarp::os::RateThread::start();
}

bool rd::RdSDLInputManager::stop()
{
    //-- Stop input thread
    yarp::os::RateThread::stop();
    return true;
}

bool rd::RdSDLInputManager::isStopped()
{
    return this->isStopped();
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

rd::RdSDLInputManager::RdSDLInputManager() : RateThread(UPDATE_RATE_MS)
{
    //-- Init SDL
    if (SDL_WasInit( SDL_INIT_EVENTTHREAD) == 0)
        SDL_Init(SDL_INIT_EVENTTHREAD);

    //-- Init X11 threads
    XInitThreads();
}

bool rd::RdSDLInputManager::update()
{
    //-- Check for event
    RdKey * key = NULL;
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
                return false;
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
    }

    delete key;
    key = NULL;
    return true;
}

void rd::RdSDLInputManager::run()
{
    update();
}
