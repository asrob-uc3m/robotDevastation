// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_GAME_SCREEN_HPP__
#define __RD_GAME_SCREEN_HPP__

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <cstdlib> // For some useful functions such as atexit :)
#include <iostream>
#include <string>
#include <vector>

#include "RdMentalMap.hpp"
#include "RdTarget.hpp"
#include "RdPlayer.hpp"


namespace rd{

class RdGameScreen
{
    public:
        RdGameScreen();
        void show( SDL_Surface * screen );

    private:
        bool drawPlayerUI( SDL_Surface * screen, RdPlayer player, int x, int y);
        bool drawTargetUI( SDL_Surface * screen, RdTarget target, RdPlayer player_data);
        bool drawScope( SDL_Surface * screen );

        //-- Screen constants (that should not be constant)
        /// \todo Change screen constants to other thing
        static const int SCREEN_WIDTH = 640;
        static const int SCREEN_HEIGHT = 480;

        //-- Player info constants
        static const int PLAYER_NAME_H = 15;
        static const int PLAYER_NAME_W = 65;
        static const int HEALTH_BAR_H = 10;
        static const int HEALTH_BAR_W = 80;

        //-- Target constants
        static const int TARGET_THICKNESS = 2;
        static const int TARGET_TEXT_BOX_HEIGHT = 15;
        static const int TARGET_HEALTH_BAR_H = 5;

        //-- Scope constants
        static const int SCOPE_VERT_W = 4;
        static const int SCOPE_VERT_H = 50;
        static const int SCOPE_VERT_H_SPACE = 20;

        static const int SCOPE_HORIZ_W = 50;
        static const int SCOPE_HORIZ_H = 4;
        static const int SCOPE_HORIZ_W_SPACE = 20;

        TTF_Font *player_font, *target_font;
        static const SDL_Color greencolor;
        static const SDL_Color redcolor;

        RdMentalMap * mentalMap;

};

}

#endif //-- __RD_GAME_SCREEN_HPP__
