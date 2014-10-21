#include "RdKeyboardManager.hpp"


const int rd::RdKeyboardManager::UPDATE_RATE_MS = 20;


rd::RdKeyboardManager::RdKeyboardManager() : RateThread(UPDATE_RATE_MS)
{
    //-- Init SDL
    if (SDL_WasInit( SDL_INIT_EVENTTHREAD) == 0)
        SDL_Init(SDL_INIT_EVENTTHREAD);

}

rd::RdKeyboardManager::~RdKeyboardManager()
{
    //-- Stop this thread
    this->stop();

    //-- Close SDL
    //! @todo Close SDL for keyboard input
}

bool rd::RdKeyboardManager::start()
{
    //-- Start input thread
    return yarp::os::RateThread::start();
}

bool rd::RdKeyboardManager::stop()
{
    //-- Stop input thread
    yarp::os::RateThread::stop();
	return true;
}

bool rd::RdKeyboardManager::setInputEventListeners(std::vector<rd::RdInputEventListener *> *listeners)
{
    this->listeners = listeners;
	return true;
}

bool rd::RdKeyboardManager::update()
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

            for ( int i = 0; i < (int)listeners->size(); i++)
                listeners->at(i)->onKeyDown(*key);

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

            for ( int i = 0; i < (int)listeners->size(); i++)
                listeners->at(i)->onKeyUp(*key);

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

void rd::RdKeyboardManager::run()
{
    update();
}
