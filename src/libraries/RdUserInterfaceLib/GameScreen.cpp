
#include "GameScreen.hpp"

const std::string rd::GameScreen::PARAM_CAMERA_FRAME = "camera_frame";
const std::string rd::GameScreen::PARAM_MYSELF = "myself";
const std::string rd::GameScreen::PARAM_PLAYERS = "players";
const std::string rd::GameScreen::PARAM_TARGETS = "targets";
const std::string rd::GameScreen::PARAM_WEAPON = "weapon";

const SDL_Color rd::GameScreen::greencolor = {0, 255, 0, 0};
const SDL_Color rd::GameScreen::redcolor =   {255, 0, 0, 0};
const SDL_Color rd::GameScreen::bluecolor =  {0, 0, 255, 0};

rd::GameScreen::GameScreen()
{
    update_required = true;
    w = 640; h = 480;
    camera_frame_surface = NULL;
}

bool rd::GameScreen::init()
{
    //-- Configure Resourcefinder to ind the real path to the resources
    yarp::os::ResourceFinder rf;
    rf.setDefaultContext("robotDevastation");
    rf.setDefaultConfigFile("robotDevastation.ini");

    //-- Load the font(s)
    std::string font_name("../fonts/FreeMono.ttf");
    player_font = TTF_OpenFont(rf.findFileByName(font_name).c_str(), 12);
    if (player_font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", rf.findFileByName(font_name).c_str(), TTF_GetError());
        return false;
    }

    target_font = TTF_OpenFont(rf.findFileByName(font_name).c_str(), 12);
    if (target_font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", rf.findFileByName(font_name).c_str(), TTF_GetError());
        return false;
    }

    weapon_font = TTF_OpenFont(rf.findFileByName(font_name).c_str(), 12);
    if (weapon_font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", rf.findFileByName(font_name).c_str(), TTF_GetError());
        return false;
    }

    update_required = true;
    return true;
}

bool rd::GameScreen::cleanup()
{
    if (camera_frame_surface!=NULL)
        SDL_FreeSurface(camera_frame_surface);

    camera_frame_surface = NULL;

    return true;
}


bool rd::GameScreen::drawScreen(void *screen)
{
    SDL_Surface * sdl_screen = (SDL_Surface *)screen;

    if (update_required)
    {
        //-- Clear screen
        SDL_FillRect(sdl_screen, NULL, 0x00000000);

        //-- Draw camera frame
        if (camera_frame_surface!=NULL)
        {
            SDL_Rect camera_frame_rect = {0,0, camera_frame_surface->w, camera_frame_surface->h};
            SDL_BlitSurface(camera_frame_surface, NULL, sdl_screen, &camera_frame_rect);
        }
        else
            RD_WARNING("No camera frame received yet\n");

        //-- Draw enemies
        for (int i = 0; i < (int) targets.size(); i++)
        {
            int target_id = targets[i].getPlayerId();
            int player_index = -1;

            for (int j = 0; j < (int) players.size(); j++)
                if (target_id == players[j].getId())
                    player_index = j;

            //-- If not found, ignore current target on HUD
            if (player_index == -1)
                continue;

            drawTargetUI(sdl_screen, targets[i], players[player_index]);
        }

        //-- Draw players
        for ( int i = 0; i < (int) players.size(); i++)
        {
            drawPlayerUI(sdl_screen, players[i], 5, 10+i*(PLAYER_NAME_H+3));
        }

        //-- Draw scope:
        drawScope(sdl_screen);

        //-- Draw user interface with user health, weapon and ammo
        drawUserUI(sdl_screen, myself, current_weapon);

        update_required = false;
    }

    return true;
}

rd::GameScreen::~GameScreen()
{

}

bool rd::GameScreen::update(std::string parameter, std::string value)
{
    RD_ERROR("No string parameter %s exists.\n", parameter.c_str());
    return false;
}

bool rd::GameScreen::update(std::string parameter, rd::RdPlayer value)
{
    if (parameter == PARAM_MYSELF)
    {
        myself = value;
        update_required = true;
        return true;
    }

    RD_ERROR("No RdPlayer parameter %s exists.\n", parameter.c_str());
    return false;
}

bool rd::GameScreen::update(std::string parameter, std::vector<rd::RdPlayer> value)
{
    if (parameter == PARAM_PLAYERS)
    {
        players = value;
        update_required = true;
        return true;
    }

    RD_ERROR("No vector<RdPlayer> parameter %s exists.\n", parameter.c_str());
    return false;
}

bool rd::GameScreen::update(std::string parameter, std::vector<rd::RdTarget> value)
{
    if (parameter == PARAM_TARGETS)
    {
        targets = value;
        update_required = true;
        return true;
    }

    RD_ERROR("No vector<RdTarget> parameter %s exists.\n", parameter.c_str());
    return false;
}

bool rd::GameScreen::update(std::string parameter, rd::RdWeapon value)
{
    if (parameter == PARAM_WEAPON)
    {
        current_weapon = value;
        update_required = true;
        return true;
    }

    RD_ERROR("No RdWeapom parameter %s exists.\n", parameter.c_str());
    return false;
}

bool rd::GameScreen::update(std::string parameter, rd::RdImage value)
{    if (value.width() == 0 || value.height() == 0)
    {
        RD_ERROR("Invalid image");
        return false;
    }

    if (parameter == PARAM_CAMERA_FRAME)
    {
        if (camera_frame_surface!=NULL)
        {
            SDL_FreeSurface(camera_frame_surface);
            camera_frame_surface=NULL;
        }

        camera_frame = RdImage(value);
        camera_frame_surface = RdImage2SDLImage(camera_frame);
        if (camera_frame_surface==NULL)
        {
            RD_ERROR("Error converting RdImage to SDL\n");
            return false;
        }
        w = camera_frame_surface->w;
        h = camera_frame_surface->h;
        update_required = true;
        return true;
    }

    RD_ERROR("No RdImage parameter %s exists.\n", parameter.c_str());
     return false;
}

bool rd::GameScreen::drawUserUI(SDL_Surface *screen, rd::RdPlayer user, rd::RdWeapon weapon)
{
    //-- User health bar:
    //--------------------------------------------------------------------------------------------
    int bar_height = SCREEN_HEIGHT - USER_HEALTH_MARGIN_Y - USER_HEALTH_BOTTOM_Y;
    int current_bar_height = (int)( user.getHealth() / (float) user.getMaxHealth() * bar_height);

    SDL_Rect health_bar = { SCREEN_WIDTH - USER_HEALTH_MARGIN_X - USER_HEALTH_W, USER_HEALTH_MARGIN_Y,
                            USER_HEALTH_W, bar_height};
    SDL_Rect current_health_bar = { health_bar.x, health_bar.y + (bar_height-current_bar_height),
                                    health_bar.w, current_bar_height};

    SDL_FillRect(screen, &health_bar, SDL_MapRGB(screen->format, 0, 0, 127));
    SDL_FillRect(screen, &current_health_bar, SDL_MapRGB(screen->format, 0, 0, 255));

    //-- Weapon data:
    //---------------------------------------------------------------------------------------------
    //-- Ammo bar:
    int current_ammo_width = (int)( weapon.getCurrentAmmo() / (float) weapon.getMaxAmmo() * AMMO_BAR_W);

    SDL_Rect ammo_bar = { health_bar.x - AMMO_BAR_MARGIN_X - AMMO_BAR_W,
                          health_bar.y + health_bar.h + AMMO_BAR_MARGIN_Y,
                          AMMO_BAR_W, AMMO_BAR_H };
    SDL_Rect current_ammo_bar = { ammo_bar.x + (AMMO_BAR_W - current_ammo_width),
                                  ammo_bar.y,
                                  current_ammo_width, ammo_bar.h};

    SDL_FillRect(screen, &ammo_bar, SDL_MapRGB(screen->format, 0, 0, 127));
    SDL_FillRect(screen, &current_ammo_bar, SDL_MapRGB(screen->format, 0, 0, 255));

    //-- Weapon name & ammo text
    SDL_Surface * name_surface = TTF_RenderText_Solid(weapon_font, weapon.getName().c_str(), bluecolor);
    SDL_Rect name_rect = { ammo_bar.x, ammo_bar.y - 5 - WEAPON_NAME_HEIGHT, WEAPON_NAME_WIDTH, WEAPON_NAME_HEIGHT};
    SDL_Rect text_rect_src = {0, 0, WEAPON_NAME_WIDTH, WEAPON_NAME_HEIGHT};
    SDL_BlitSurface(name_surface, &text_rect_src, screen, &name_rect);

    std::stringstream sstream;
    sstream << weapon.getCurrentAmmo() << "/" << weapon.getMaxAmmo();
    SDL_Surface * ammo_surface = TTF_RenderText_Solid(weapon_font, sstream.str().c_str(), bluecolor);
    SDL_Rect ammo_rect = { ammo_bar.x + ammo_bar.w - AMMO_TEXT_WIDTH, ammo_bar.y - 5 - AMMO_TEXT_HEIGHT,
                           AMMO_TEXT_WIDTH, AMMO_TEXT_HEIGHT};
    SDL_Rect ammo_rect_src = {0, 0, AMMO_TEXT_WIDTH, AMMO_TEXT_HEIGHT};
    SDL_BlitSurface(ammo_surface, &ammo_rect_src, screen, &ammo_rect);

    SDL_FreeSurface(name_surface);
    SDL_FreeSurface(ammo_surface);
    return true;
}

bool rd::GameScreen::drawPlayerUI(SDL_Surface *screen, RdPlayer player, int x, int y)
{
    //-- Player interface:
    SDL_Surface * text_surface = TTF_RenderText_Solid(player_font, player.getName().c_str(), greencolor );
    SDL_Rect text_rect = {x, y, PLAYER_NAME_W, PLAYER_NAME_H};
    SDL_BlitSurface(text_surface, NULL, screen, &text_rect);

    //-- Health bar:
    SDL_Rect health_bar = {x+PLAYER_NAME_W, y, HEALTH_BAR_W, HEALTH_BAR_H };
    SDL_Rect current_health = {x+PLAYER_NAME_W,
                               y,
                               (int)(HEALTH_BAR_W*player.getHealth() / (float) player.getMaxHealth()),
                               HEALTH_BAR_H };

    SDL_FillRect(screen, &health_bar, SDL_MapRGB(screen->format, 0, 127, 0));
    SDL_FillRect(screen, &current_health, SDL_MapRGB(screen->format, 0, 255, 0));

    SDL_FreeSurface(text_surface);
    return true;
}

bool rd::GameScreen::drawTargetUI(SDL_Surface *screen, RdTarget target, RdPlayer player_data)
{
    //-- Check that the dimensions of the target are feasible
    if (target.getDimensions().x < 0 || target.getDimensions().y < 0 ||
            target.getPos().x < 0 || target.getPos().y < 0)
    {
        RD_ERROR("Trying to draw an invalid target: %s\n", player_data.getName().c_str());
        return false;
    }

    //-- Convert target coordinates from center+dimensions to corner+dimensions
    int target_top_left_x = target.getPos().x - target.getDimensions().x / 2;
    int target_top_left_y = target.getPos().y - target.getDimensions().y / 2;
    int target_dimensions_x = target.getDimensions().x;
    int target_dimensions_y = target.getDimensions().y;

    //-- Put enclosing box, which is made of 4 rectangles (one for each side):
    SDL_Rect target_rect_top = { target_top_left_x, target_top_left_y,
                                target_dimensions_x, TARGET_THICKNESS };
    SDL_FillRect(screen, &target_rect_top, SDL_MapRGB(screen->format, 255, 0, 0));

    SDL_Rect target_rect_bottom = { target_top_left_x, target_top_left_y + target_dimensions_y - TARGET_THICKNESS,
                                   target_dimensions_x, TARGET_THICKNESS};
    SDL_FillRect(screen, &target_rect_bottom, SDL_MapRGB(screen->format, 255, 0, 0));

    SDL_Rect target_rect_left = { target_top_left_x, target_top_left_y,
                                 TARGET_THICKNESS, target_dimensions_y};
    SDL_FillRect(screen, &target_rect_left, SDL_MapRGB(screen->format, 255, 0, 0));

    SDL_Rect target_rect_right = { target_top_left_x + target_dimensions_x - TARGET_THICKNESS, target_top_left_y,
                                  TARGET_THICKNESS, target_dimensions_y};
    SDL_FillRect(screen, &target_rect_right, SDL_MapRGB(screen->format, 255, 0, 0));

    //-- Put target name:
    SDL_Surface * text_surface = TTF_RenderText_Solid(target_font, player_data.getName().c_str(), redcolor);
    SDL_Rect text_rect = { target_top_left_x, target_top_left_y - TARGET_TEXT_BOX_HEIGHT,
                           target_dimensions_x, TARGET_TEXT_BOX_HEIGHT};
    SDL_Rect source_rect = {0, 0, target.getDimensions().x, TARGET_TEXT_BOX_HEIGHT};
    SDL_BlitSurface(text_surface, &source_rect, screen, &text_rect);

    //-- Put target health bar:
    SDL_Rect health_bar = { target_top_left_x,
                            target_top_left_y + target_dimensions_y + TARGET_HEALTH_BAR_H,
                            target_dimensions_x,
                            TARGET_HEALTH_BAR_H };
    SDL_FillRect(screen, &health_bar, SDL_MapRGB(screen->format, 127, 0, 0));

    SDL_Rect current_health = { target_top_left_x,
                                target_top_left_y + target_dimensions_y + TARGET_HEALTH_BAR_H,
                                (int)(target_dimensions_x *player_data.getHealth() / (float) player_data.getMaxHealth()),
                                TARGET_HEALTH_BAR_H };
    SDL_FillRect(screen, &current_health, SDL_MapRGB(screen->format, 255, 0, 0));

    SDL_FreeSurface(text_surface);
    return true;
}

bool rd::GameScreen::drawScope(SDL_Surface *screen)
{
    SDL_Rect scope_v_rect_top = { (int)(SCREEN_WIDTH  / 2 - SCOPE_VERT_W  / 2),
                                  (int)(SCREEN_HEIGHT / 2 - SCOPE_VERT_H  / 2),
                                  SCOPE_VERT_W,
                                  (int)(SCOPE_VERT_H / 2 - SCOPE_VERT_H_SPACE / 2) };
    SDL_FillRect(screen, &scope_v_rect_top, SDL_MapRGB(screen->format, 0, 0, 255));

    SDL_Rect scope_v_rect_bottom = { (int)(SCREEN_WIDTH  / 2 - SCOPE_VERT_W  / 2),
                                     (int)(SCREEN_HEIGHT / 2 + SCOPE_VERT_H_SPACE / 2),
                                     SCOPE_VERT_W,
                                     (int)(SCOPE_VERT_H / 2 - SCOPE_VERT_H_SPACE / 2) };
    SDL_FillRect(screen, &scope_v_rect_bottom, SDL_MapRGB(screen->format, 0, 0, 255));

    SDL_Rect scope_h_rect_left = { (int) (SCREEN_WIDTH  / 2 - SCOPE_HORIZ_W  / 2),
                                   (int) (SCREEN_HEIGHT / 2 - SCOPE_HORIZ_H  / 2),
                                   (int) (SCOPE_HORIZ_W / 2 - SCOPE_HORIZ_W_SPACE / 2),
                                   SCOPE_HORIZ_H };
    SDL_FillRect(screen, &scope_h_rect_left, SDL_MapRGB(screen->format, 0, 0, 255));

    SDL_Rect scope_h_rect_right = { (int) (SCREEN_WIDTH  / 2 + SCOPE_HORIZ_W_SPACE / 2),
                                    (int) (SCREEN_HEIGHT / 2 - SCOPE_HORIZ_H  / 2),
                                    (int) (SCOPE_HORIZ_W / 2 - SCOPE_HORIZ_W_SPACE / 2),
                                    SCOPE_HORIZ_H };
    SDL_FillRect(screen, &scope_h_rect_right, SDL_MapRGB(screen->format, 0, 0, 255));

    return true;
}

