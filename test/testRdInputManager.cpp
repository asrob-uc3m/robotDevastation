/***
 * testRdInputManager
 *
 * Test class for testing the input manager and listener classes
 *
 * Commands:
 * ARROWS -  control the scope bars
 * SPACE or 0 - shoot
 * ESCAPE or q - exit
 *
 ***/
#include <RdInputManager.hpp>
#include <RdSDLInputManager.hpp>
#include <RdInputEventListener.hpp>
#include <SDLAudioManager.hpp>

#include <SDL.h>
#include <SDL_ttf.h>

#include <cstdlib> // atexit()

using namespace rd;

bool finished = false;
int x_pos = 0;
int y_pos = 0;

class RdTestEventListener : public RdInputEventListener
{
    public:
        RdTestEventListener()
        {
            SDLAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("SDL");

            if (audioManager == NULL)
            {
                RD_ERROR("Could not get AudioManager\n");
                exit(1);
            }

            audioManager->load( "../sounds/01_milshot.wav", "shoot", SDLAudioManager::FX);
        }

        ~RdTestEventListener()
        {
            SDLAudioManager::destroyAudioManager();
        }

        virtual bool onKeyDown(const RdKey & k)
        {
            if ( k.isControlKey() )
            {
                RD_SUCCESS( "Control key with code %d pressed!\n", k.getValue() );

                if ( k.getValue() == RdKey::KEY_SPACE)
                {
                    audioManager->play("shoot", false);
                    RD_SUCCESS("Shoot!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ESCAPE)
                {
                    finished = true;
                    RD_SUCCESS("Exit!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_LEFT)
                {
                    x_pos -= 20;
                    if (x_pos < 0 )
                        x_pos = 0;

                    RD_SUCCESS("Reduce x!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_RIGHT)
                {
                    x_pos += 20;
                    if (x_pos > 640 )
                        x_pos = 640;

                    RD_SUCCESS("Increase x!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_UP)
                {
                    y_pos -= 20;
                    if (y_pos < 0 )
                        y_pos = 0;

                    RD_SUCCESS("Reduce y!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_DOWN)
                {
                    y_pos += 20;
                    if (y_pos > 640 )
                        y_pos = 640;

                    RD_SUCCESS("Increase y!\n");
                }

            }
            else if (k.isPrintable() )
            {
                RD_SUCCESS( "Key \"%c\" was pressed!\n", k.getChar() );

                if ( k.getChar() == '0')
                {
                    audioManager->play("shoot", false);
                    RD_SUCCESS("Shoot!\n");
                }
                else if ( k.getChar() == 'q')
                {
                    finished = true;
                    RD_SUCCESS("Exit!\n");
                }
            }
        }

        virtual bool onKeyUp(const RdKey & k)
        {
            if ( k.isControlKey() )
            {
                RD_SUCCESS( "Control key with code %d released!\n", k.getValue() );
                if ( k.getValue() == RdKey::KEY_SPACE)
                {
                    RD_SUCCESS("Released shoot!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_LEFT)
                {
                    RD_SUCCESS("Released left!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_RIGHT)
                {
                    RD_SUCCESS("Released right!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_UP)
                {
                    RD_SUCCESS("Released up!\n");
                }
                else if ( k.getValue() == RdKey::KEY_ARROW_DOWN)
                {
                    RD_SUCCESS("Release down!\n");
                }
            }
            else if (k.isPrintable() )
            {
                RD_SUCCESS( "Key \"%c\" was released!\n", k.getChar() );

            }
        }

        virtual bool onWindowEvent(const RdWindowEvent & event)
        {
            if ( event.getEvent() == RdWindowEvent::WINDOW_UNKNOWN )
            {
                RD_WARNING("Window event not implemented!\n");
            }
            else
            {
                RD_SUCCESS( "Triggered window event with code %d!\n", event.getEvent() );

                if ( event.getEvent() == RdWindowEvent::WINDOW_CLOSE )
                {
                    finished = true;
                    RD_SUCCESS("Exit!\n");
                }
            }
        }

    private:
        AudioManager * audioManager;
};

int main(void)
{
    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
    }
    atexit(SDL_Quit); // Clean it up nicely :) FIXME: #70

    //-- Init screen
    SDL_Window * window = SDL_CreateWindow("Robot Devastation",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640, 480,
                              0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
    if (!window) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    //-- Screen surface
    SDL_Surface * screen = SDL_GetWindowSurface( window );

    //-- Get a (SDL) inputManager
    RdSDLInputManager::RegisterManager();
    RdInputManager * inputManager = RdInputManager::getInputManager("SDL");
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
    {
        //-- Clear screen:
        SDL_FillRect(screen, NULL, 0x000000);

        //-- Draw a bar that can be controlled with the keyboard
        SDL_Rect ybar = {0, y_pos, 640, 2};
        SDL_FillRect(screen, &ybar, SDL_MapRGB(screen->format, 255, 0, 0));

        SDL_Rect xbar = {x_pos, 0, 2, 480};
        SDL_FillRect(screen, &xbar, SDL_MapRGB(screen->format, 255, 0, 0));

        //-- Draw dummy screen to enable input events
        SDL_UpdateWindowSurface(window); //Refresh the screen
        SDL_Delay(20); //Wait a bit :)

        SDL_PumpEvents();
    }

    RdInputManager::destroyInputManager();

    return 0;
}
