#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <cstdlib> // For some useful functions such as atexit :)
#include <iostream>
#include <yarp/os/ResourceFinder.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define COLORKEY 255, 0, 255 //Your Transparent colour



int main(void)
{
    //-- Configure Resourcefinder to ind the real path to the resources
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
    return false;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    //-- Init ttf
    if (TTF_Init() == -1)
    {
      printf("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
      return false;
    }

    //-- Screen surface
    SDL_Surface * screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 16, SDL_DOUBLEBUF);
    if (!screen) {
        fprintf(stderr, "Unable to set video mode: %s\n", SDL_GetError());
        return false;
    }

    //-- Load the font
    std::string font_name("../fonts/FreeMono.ttf");
    TTF_Font *font = TTF_OpenFont(rf.findFileByName(font_name).c_str(), 28);
    if (font == NULL){
        printf("Unable to load font: %s %s \n", rf.findFileByName(font_name).c_str(), TTF_GetError());
        return false;
    }

    //-- Draw text
    SDL_Color redcolor = {255,0,0,0};

    SDL_Surface * text_surface = TTF_RenderText_Solid(font, "ROBODEVASTATION", redcolor);

    int i = 0, j = 0;
    while( j < 1)
    {
        while ( i < SCREEN_WIDTH )
        {
            SDL_FillRect(screen, NULL, 0x000000);

            SDL_Rect health_bar, bar2, bar3;
            health_bar.x = 5; health_bar.y = 40;
            health_bar.w = i; health_bar.h = 20;
            bar2 = health_bar; bar2.y = 70;
            bar3 = health_bar; bar3.y = 100;
            if( SDL_FillRect(screen, &health_bar, SDL_MapRGB(screen->format, 255, 0, 0)) == -1)
                std::cerr << "Could not print bar..." << std::endl;
            if( SDL_FillRect(screen, &bar2, SDL_MapRGB(screen->format, 0, 255, 0)) == -1)
                std::cerr << "Could not print bar..." << std::endl;
            if( SDL_FillRect(screen, &bar3, SDL_MapRGB(screen->format, 0, 0, 255)) == -1)
                std::cerr << "Could not print bar..." << std::endl;

            //Setup the location on the screen to blit to
            SDL_Rect rect = { i+5, 5, i+100, 50 };

            //Blit text_surface surface to the screen surface
            SDL_BlitSurface(text_surface, NULL, screen, &rect);

            SDL_Flip(screen); //Refresh the screen
            SDL_Delay(20); //Wait a bit :)

            i++;
        }
    j++;
    }

    //Free the text_surface surface
    SDL_FreeSurface(screen);
    SDL_FreeSurface(text_surface);

}
