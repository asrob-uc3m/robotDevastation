#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <cstdlib> // For some useful functions such as atexit :)
#include <iostream>
#include <string>
#include <vector>

#include "RdMentalMap.hpp"
#include "RdTarget.hpp"
#include "RdPlayer.hpp"
#include "RdGameScreen.hpp"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define COLORKEY 255, 0, 255 //Your Transparent colour

using namespace rd;

int main(void)
{
    //-- Screen surface
    SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }

    //-- Create a mental map with some info:
    RdMentalMap mentalMap(0);

    std::vector<RdPlayer> players;
    players.push_back(RdPlayer(0, "Myself", 90, 100, 0, 0));
    players.push_back(RdPlayer(1, "Enemy1", 50, 100, 1, 0));
    players.push_back(RdPlayer(2, "Enemy2", 75, 100, 1, 0));
    mentalMap.updatePlayers(players);

    std::vector<RdTarget> targets;
    targets.push_back(RdTarget(1, RdVector2d(100, 100), RdVector2d(50, 50)));
    targets.push_back(RdTarget(2, RdVector2d(400, 200), RdVector2d(100, 100)));
    mentalMap.updateTargets(targets);


    RdGameScreen gameScreen;
    gameScreen.setMentalMap(&mentalMap);

    //-- Draw interface:
    //---------------------------------------------------------------------------------
//  SDL_FillRect(screen, NULL, 0xFFFFFFFF);
    SDL_FillRect(screen, NULL, 0x00000000);

    gameScreen.show(screen);

    //-- Show interface
    SDL_Flip(screen);
    SDL_Delay(2000);
    std::cin.get();

    //Free the text_surface surface
    SDL_FreeSurface(screen);
}



