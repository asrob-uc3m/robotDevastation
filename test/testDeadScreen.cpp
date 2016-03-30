#include <SDL/SDL.h>
#include "DeadScreen.hpp"

using namespace rd;

int main()
{
    DeadScreen screen;

    for (int i = 0; i < 200; i++)
    {
        screen.show();
        SDL_Delay(20); //Wait a bit :)
    }
}
