#include <SDL/SDL.h>
#include "SDLUtils.hpp"
#include "InitScreen.hpp"

using namespace rd;

int main()
{
    initSDL();

    InitScreen screen;
    screen.init();

    for (int i = 0; i < 200; i++)
    {
        screen.show();
        SDL_Delay(20); //Wait a bit :)
    }
    screen.cleanup();

    cleanupSDL();
}
