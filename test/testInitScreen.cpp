#include <SDL/SDL.h>
#include "InitScreen.hpp"

using namespace rd;

int main()
{
    InitScreen screen;

    for (int i = 0; i < 200; i++)
    {
        screen.show();
        SDL_Delay(20); //Wait a bit :)
    }
}
