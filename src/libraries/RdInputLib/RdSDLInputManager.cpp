#include "RdSDLInputManager.hpp"

//-- Initialize static members
rd::RdSDLInputManager * rd::RdSDLInputManager::uniqueInstance = NULL;
const std::string rd::RdSDLInputManager::id = "SDL";

bool rd::RdSDLInputManager::start()
{

}

bool rd::RdSDLInputManager::stop()
{

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
    uniqueInstance = NULL;
}

rd::RdSDLInputManager::RdSDLInputManager()
{

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

void rd::RdSDLInputManager::run()
{
    update();
}
