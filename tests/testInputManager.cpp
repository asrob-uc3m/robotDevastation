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

#include <yarp/os/LogStream.h>

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
                yError() << "Could not get AudioManager";
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
                yInfo() << "Control key with code" << k.getValue() << "pressed!";

                if ( k.getValue() == Key::KEY_SPACE)
                {
                    audioManager->play("shoot", false);
                    yInfo() << "Shoot!";
                }
                else if ( k.getValue() == Key::KEY_ESCAPE)
                {
                    finished = true;
                    yInfo() << "Exit!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_LEFT)
                {
                    x_pos -= 20;
                    if (x_pos < 0 )
                        x_pos = 0;

                    yInfo() << "Reduce x!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_RIGHT)
                {
                    x_pos += 20;
                    if (x_pos > 640 )
                        x_pos = 640;

                    yInfo() << "Increase x!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_UP)
                {
                    y_pos -= 20;
                    if (y_pos < 0 )
                        y_pos = 0;

                    yInfo() << "Reduce y!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_DOWN)
                {
                    y_pos += 20;
                    if (y_pos > 640 )
                        y_pos = 640;

                    yInfo() << "Increase y!";
                }

            }
            else if (k.isPrintable() )
            {
                yInfo() << "Key" << k.getChar() << "was pressed!";

                if ( k.getChar() == '0')
                {
                    audioManager->play("shoot", false);
                    yInfo() << "Shoot!";
                }
                else if ( k.getChar() == 'q')
                {
                    finished = true;
                    yInfo() << "Exit!";
                }
            }
            return true;
        }

        virtual bool onKeyUp(const Key & k)
        {
            if ( k.isControlKey() )
            {
                yInfo() << "Control key with code" << k.getValue() << "released!";
                if ( k.getValue() == Key::KEY_SPACE)
                {
                    yInfo() << "Released shoot!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_LEFT)
                {
                    yInfo() << "Released left!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_RIGHT)
                {
                    yInfo() << "Released right!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_UP)
                {
                    yInfo() << "Released up!";
                }
                else if ( k.getValue() == Key::KEY_ARROW_DOWN)
                {
                    yInfo() << "Release down!";
                }
            }
            else if (k.isPrintable() )
            {
                yInfo() << "Key" << k.getChar() << "was released!";

            }
            return true;
        }

        virtual bool onWindowEvent(const WindowEvent & event)
        {
            if ( event.getEvent() == WindowEvent::WINDOW_UNKNOWN )
            {
                yWarning() << "Window event not implemented!";
            }
            else
            {
                yInfo() << "Triggered window event with code" << event.getEvent();

                if ( event.getEvent() == WindowEvent::WINDOW_CLOSE )
                {
                    finished = true;
                    yInfo() << "Exit!";
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
        yError() << "Could not get AudioManager";
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
