#include <SDL.h>
#include "SDLUtils.hpp"
#include "InitScreen.hpp"

using namespace rd;

int main()
{
    if( ! initSDL() )
        return 1;

    InitScreen screen;
    if( ! screen.init() )
        return 1;

    for (int i = 0; i < 200; i++)
    {
        if( ! screen.show() )
            return 1;
        SDL_Delay(20); //Wait a bit :)
    }
    if( ! screen.cleanup() )
        return 1;

    if( ! cleanupSDL() )
        return 1;

    return 0;
}
