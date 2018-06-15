// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

/***
 * testInputManager
 *
 * Test class for testing the input manager and listener classes
 *
 * Commands:
 * ARROWS -  control the scope bars
 * SPACE or 0 - shoot
 * ESCAPE or q - exit
 *
 ***/
#include <cstdio>
#include <cstdlib> // atexit()

#include "InputManager.hpp"
#include "SDLInputManager.hpp"
#include "InputEventListener.hpp"
#include "SDLAudioManager.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <ColorDebug.h>

namespace rd
{

namespace test
{

bool finished = false;
int x_pos = 0;
int y_pos = 0;

class TestEventListener : public InputEventListener
{
    public:
        TestEventListener()
        {
            SDLAudioManager::RegisterManager();
            audioManager = AudioManager::getAudioManager("SDL");

            if (audioManager == NULL)
            {
                CD_ERROR("Could not get AudioManager\n");
                exit(1);
            }

            audioManager->load( "../sounds/01_milshot.wav", "shoot", SDLAudioManager::FX);
        }

        virtual ~TestEventListener()
        {
            SDLAudioManager::destroyAudioManager();
        }

        virtual bool onKeyDown(const Key & k)
        {
            if ( k.isControlKey() )
            {
                CD_SUCCESS( "Control key with code %d pressed!\n", k.getValue() );

                if ( k.getValue() == Key::KEY_SPACE)
                {
                    audioManager->play("shoot", false);
                    CD_SUCCESS("Shoot!\n");
                }
                else if ( k.getValue() == Key::KEY_ESCAPE)
                {
                    finished = true;
                    CD_SUCCESS("Exit!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_LEFT)
                {
                    x_pos -= 20;
                    if (x_pos < 0 )
                        x_pos = 0;

                    CD_SUCCESS("Reduce x!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_RIGHT)
                {
                    x_pos += 20;
                    if (x_pos > 640 )
                        x_pos = 640;

                    CD_SUCCESS("Increase x!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_UP)
                {
                    y_pos -= 20;
                    if (y_pos < 0 )
                        y_pos = 0;

                    CD_SUCCESS("Reduce y!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_DOWN)
                {
                    y_pos += 20;
                    if (y_pos > 640 )
                        y_pos = 640;

                    CD_SUCCESS("Increase y!\n");
                }

            }
            else if (k.isPrintable() )
            {
                CD_SUCCESS( "Key \"%c\" was pressed!\n", k.getChar() );

                if ( k.getChar() == '0')
                {
                    audioManager->play("shoot", false);
                    CD_SUCCESS("Shoot!\n");
                }
                else if ( k.getChar() == 'q')
                {
                    finished = true;
                    CD_SUCCESS("Exit!\n");
                }
            }
            return true;
        }

        virtual bool onKeyUp(const Key & k)
        {
            if ( k.isControlKey() )
            {
                CD_SUCCESS( "Control key with code %d released!\n", k.getValue() );
                if ( k.getValue() == Key::KEY_SPACE)
                {
                    CD_SUCCESS("Released shoot!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_LEFT)
                {
                    CD_SUCCESS("Released left!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_RIGHT)
                {
                    CD_SUCCESS("Released right!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_UP)
                {
                    CD_SUCCESS("Released up!\n");
                }
                else if ( k.getValue() == Key::KEY_ARROW_DOWN)
                {
                    CD_SUCCESS("Release down!\n");
                }
            }
            else if (k.isPrintable() )
            {
                CD_SUCCESS( "Key \"%c\" was released!\n", k.getChar() );

            }
            return true;
        }

        virtual bool onWindowEvent(const WindowEvent & event)
        {
            if ( event.getEvent() == WindowEvent::WINDOW_UNKNOWN )
            {
                CD_WARNING("Window event not implemented!\n");
            }
            else
            {
                CD_SUCCESS( "Triggered window event with code %d!\n", event.getEvent() );

                if ( event.getEvent() == WindowEvent::WINDOW_CLOSE )
                {
                    finished = true;
                    CD_SUCCESS("Exit!\n");
                }
            }
            return true;
        }

    private:
        AudioManager * audioManager;
};

}  // namespace test

}  // namespace rd

int main(void)
{
    using namespace rd;
    using namespace rd::test;

    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    std::atexit(SDL_Quit); // Clean it up nicely :)

    //-- Init screen
    SDL_Window * window = SDL_CreateWindow("Robot Devastation",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              640, 480,
                              0);  // 16, SDL_DOUBLEBUF // SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL
    if (!window) {
        std::fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }
    //-- Screen surface
    SDL_Surface * screen = SDL_GetWindowSurface( window );

    //-- Get a (SDL) inputManager
    SDLInputManager::RegisterManager();
    InputManager * inputManager = InputManager::getInputManager("SDL");
    if (inputManager == NULL)
    {
        CD_ERROR("Could not get AudioManager\n");
        return 1;
    }

    //-- Set event listener
    TestEventListener listener;
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

    InputManager::destroyInputManager();

    return 0;
}
