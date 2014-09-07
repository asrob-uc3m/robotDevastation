#include <RdInputManager.hpp>
#include <RdInputEventListener.hpp>
#include <RdAudioManager.hpp>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

using namespace rd;

bool finished = false;

class RdTestEventListener : public RdInputEventListener
{
    public:
        RdTestEventListener()
        {
            audioManager = RdAudioManager::getAudioManager();

            if (audioManager == NULL)
            {
                RD_ERROR("Could not get AudioManager\n");
                exit(1);
            }

            audioManager->load( "../../share/sounds/01_milshot.wav", "shoot", RdAudioManager::FX);
        }

        ~RdTestEventListener()
        {
            RdAudioManager::destroyAudioManager();
        }

        virtual bool onKeyPressed(RdKey k)
        {
            if ( k.isControlKey() )
            {
                RD_SUCCESS( "Control key with code %d pressed!\n", k.getValue() );

                if ( k.getValue() == RdKey::KEY_SPACE)
                {
                    audioManager->playSound("shoot", false);
                    RD_SUCCESS("Shoot!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ESCAPE)
                {
                    finished = true;
                    RD_SUCCESS("Exit!\n");
                }
            }
            else if (k.isPrintable() )
            {
                RD_SUCCESS( "Key \"%c\" was pressed!\n", k.getChar() );

                if ( k.getChar() == '0')
                {
                    audioManager->playSound("shoot", false);
                    RD_SUCCESS("Shoot!\n");
                }
                else if ( k.getChar() == 'q')
                {
                    finished = true;
                    RD_SUCCESS("Exit!\n");
                }
            }
        }

    private:
        RdAudioManager * audioManager;
};

int main(void)
{
    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    //-- Screen surface
    SDL_Surface * screen = SDL_SetVideoMode(640, 480, 16, SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }

    //-- Draw dummy screen to enable input events
    SDL_Flip(screen); //Refresh the screen
    SDL_Delay(20); //Wait a bit :)


    //-- Get a inputManager
    RdInputManager * inputManager = RdInputManager::getInputManager();
    if (inputManager == NULL)
    {
        RD_ERROR("Could not get AudioManager\n");
        return 1;
    }

    //-- Set event listener
    RdTestEventListener listener;
    inputManager->addInputEventListener(&listener);
    inputManager->start();

    //-- Wait for termination
    while(!finished)
    { }

    RdInputManager::destroyInputManager();
    SDL_Quit();
}
