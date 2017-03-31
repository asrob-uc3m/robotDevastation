// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_GAME_SCREEN_HPP__
#define __RD_GAME_SCREEN_HPP__

#include <string>
#include <vector>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Target.hpp"
#include "Player.hpp"
#include "Weapon.hpp"
#include "Screen.hpp"
#include "ImageManager.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup UserInterfaceLib
 *
 * @brief The UserInterfaceLib library contains the different game screens
 */

/**
 * @ingroup UserInterfaceLib
 *
 * @brief A User Interface.
 *
 */
class GameScreen : public Screen
{
    public:
        GameScreen();
        virtual bool init();
        virtual bool cleanup();
        virtual bool drawScreen(void *screen);
        virtual ~GameScreen();
        virtual bool update(const std::string & parameter, const std::string & value);
        virtual bool update(const std::string & parameter, Player value);
        virtual bool update(const std::string & parameter, std::vector<Player> value);
        virtual bool update(const std::string & parameter, std::vector<Target> value);
        virtual bool update(const std::string & parameter, Weapon value);
        virtual bool update(const std::string & parameter, Image value);

        //-- Screen interface parameters
        static const std::string PARAM_CAMERA_FRAME;
        static const std::string PARAM_MYSELF;
        static const std::string PARAM_PLAYERS;
        static const std::string PARAM_TARGETS;
        static const std::string PARAM_WEAPON;

    private:
        bool drawUserUI(SDL_Surface * screen, Player user, Weapon weapon);
        bool drawPlayerUI( SDL_Surface * screen, Player player, int x, int y);
        bool drawTargetUI( SDL_Surface * screen, Target target, Player player_data);
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

        //-- User info display constants
        static const int USER_HEALTH_MARGIN_X = 10;
        static const int USER_HEALTH_MARGIN_Y = 20;
        static const int USER_HEALTH_W = 20;
        static const int USER_HEALTH_BOTTOM_Y = 50;

        static const int AMMO_BAR_MARGIN_X =20;
        static const int AMMO_BAR_MARGIN_Y = 20;
        static const int AMMO_BAR_W = 200;
        static const int AMMO_BAR_H = 15;

        static const int WEAPON_NAME_WIDTH = 100;
        static const int WEAPON_NAME_HEIGHT = 10;

        static const int AMMO_TEXT_WIDTH = 50;
        static const int AMMO_TEXT_HEIGHT = 10;

        //-- Refresh control
        bool update_required;

        //-- SDL stuff
        SDL_Surface * camera_frame_surface;
        TTF_Font *player_font, *target_font, *weapon_font;

        static const SDL_Color greencolor;
        static const SDL_Color redcolor;
        static const SDL_Color bluecolor;

        Player myself;
        std::vector<Player> players;
        std::vector<Target> targets;
        Weapon current_weapon;
        Image camera_frame;

};

}

#endif //-- __RD_GAME_SCREEN_HPP__
