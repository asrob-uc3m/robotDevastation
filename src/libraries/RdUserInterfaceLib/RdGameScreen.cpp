
#include "RdGameScreen.hpp"

const SDL_Color rd::RdGameScreen::greencolor = {0, 255, 0, 0};
const SDL_Color rd::RdGameScreen::redcolor =   {255, 0, 0, 0};

rd::RdGameScreen::RdGameScreen()
{
    /// \todo Fix this

    //-- Setup SDL
    //----------------------------------------------------------------------------------------
    // Load SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        RD_ERROR("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }
    atexit(SDL_Quit); // Clean it up nicely :)

    //-- Init ttf
    if (TTF_Init() == -1)
    {
      RD_ERROR("Unable to initialize SDL_ttf: %s \n", TTF_GetError());
      return;
    }

    //-- Load the font(s)
    char * font_name = "/usr/share/fonts/truetype/freefont/FreeMono.ttf";
    player_font = TTF_OpenFont(font_name, 12);
    if (player_font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", font_name, TTF_GetError());
        return;
    }

    target_font = TTF_OpenFont(font_name, 12);
    if (target_font == NULL)
    {
        RD_ERROR("Unable to load font: %s %s \n", font_name, TTF_GetError());
        return;
    }

    mentalMap = 0;
}

void rd::RdGameScreen::show(SDL_Surface *screen)
{
    //-- Get a copy of the enemies stored & draw them:
    std::vector<RdTarget> targets_stored  = mentalMap->getTargets();
    for (int i = 0; i < (int) targets_stored.size(); i++)
    {
        RdPlayer player_data = mentalMap->getPlayer(targets_stored[i].getPlayerId());
        drawTargetUI(screen, targets_stored[i], player_data);
    }

    //-- Get a copy of the players stored & draw them:
    std::vector<RdPlayer> players_stored = mentalMap->getPlayers();
    for ( int i = 0; i < (int) players_stored.size(); i++)
    {
        drawPlayerUI(screen, players_stored[i], 5, 10+i*(PLAYER_NAME_H+3));
    }

    //-- Draw scope:
    drawScope(screen);
}

void rd::RdGameScreen::setMentalMap(RdMentalMap *mentalMap)
{
    this->mentalMap = mentalMap;
}

bool rd::RdGameScreen::drawPlayerUI(SDL_Surface *screen, RdPlayer player, int x, int y)
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

bool rd::RdGameScreen::drawTargetUI(SDL_Surface *screen, RdTarget target, RdPlayer player_data)
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

bool rd::RdGameScreen::drawScope(SDL_Surface *screen)
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
