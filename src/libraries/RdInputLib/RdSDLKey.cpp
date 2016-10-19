#include "RdSDLKey.hpp"

bool rd::RdSDLKey::initialized = false;
std::map< SDL_Keycode, char> rd::RdSDLKey::sdl_printable_map = std::map<SDL_Keycode, char>();
std::map< SDL_Keycode, int>  rd::RdSDLKey::sdl_control_map = std::map<SDL_Keycode, int>();

rd::RdSDLKey::RdSDLKey(SDL_Keycode keycode)
{

    //-- Check if the key maps have been initialized
    if (!initialized)
        initLookupTables();

    printable = false;
    control = false;

    //-- Assign char value:
    if ( sdl_printable_map.find(keycode) != sdl_printable_map.end() )
    {
        char_value = sdl_printable_map.at(keycode);
        printable = true;
    }
    else
    {
        char_value = '\0';
    }

    //-- Assign key value:
    if ( sdl_control_map.find(keycode) != sdl_control_map.end() )
    {
        key_value = sdl_control_map.at(keycode);
        control = true;
    }
    else if (printable)
    {
        key_value = KEY_PRINTABLE;
    }
    else
    {
        key_value = KEY_UNKNOWN;
    }

    //-- Return feedback
    if ( !(printable || control) )
    {
        char_value = '\0';
        key_value = KEY_UNKNOWN;
        RD_ERROR("Key not supported!\n");
    }
}


bool rd::RdSDLKey::initLookupTables()
{
    if (!initialized)
    {
        sdl_printable_map[SDLK_0] = '0'; sdl_printable_map[SDLK_1] = '1'; sdl_printable_map[SDLK_2] = '2';
        sdl_printable_map[SDLK_3] = '3'; sdl_printable_map[SDLK_4] = '4'; sdl_printable_map[SDLK_5] = '5';
        sdl_printable_map[SDLK_6] = '6'; sdl_printable_map[SDLK_7] = '7'; sdl_printable_map[SDLK_8] = '8';
        sdl_printable_map[SDLK_9] = '9';

        sdl_printable_map[SDLK_a] = 'a'; sdl_printable_map[SDLK_b] = 'b'; sdl_printable_map[SDLK_c] = 'c';
        sdl_printable_map[SDLK_d] = 'd'; sdl_printable_map[SDLK_e] = 'e'; sdl_printable_map[SDLK_f] = 'f';
        sdl_printable_map[SDLK_g] = 'g'; sdl_printable_map[SDLK_h] = 'h'; sdl_printable_map[SDLK_i] = 'i';
        sdl_printable_map[SDLK_j] = 'j'; sdl_printable_map[SDLK_k] = 'k'; sdl_printable_map[SDLK_l] = 'l';
        sdl_printable_map[SDLK_m] = 'm'; sdl_printable_map[SDLK_n] = 'n'; sdl_printable_map[SDLK_o] = 'o';
        sdl_printable_map[SDLK_p] = 'p'; sdl_printable_map[SDLK_q] = 'q'; sdl_printable_map[SDLK_r] = 'r';
        sdl_printable_map[SDLK_s] = 's'; sdl_printable_map[SDLK_t] = 't'; sdl_printable_map[SDLK_u] = 'u';
        sdl_printable_map[SDLK_v] = 'v'; sdl_printable_map[SDLK_w] = 'w'; sdl_printable_map[SDLK_x] = 'x';
        sdl_printable_map[SDLK_y] = 'y'; sdl_printable_map[SDLK_z] = 'z';

        sdl_printable_map[SDLK_SPACE] = ' ';
        sdl_printable_map[SDLK_RETURN] = '\n';

        sdl_control_map[SDLK_SPACE] = rd::RdKey::KEY_SPACE;
        sdl_control_map[SDLK_ESCAPE] = rd::RdKey::KEY_ESCAPE;
        sdl_control_map[SDLK_BACKSPACE] = rd::RdKey::KEY_BACKSPACE;
        sdl_control_map[SDLK_UP] = rd::RdKey::KEY_ARROW_UP;
        sdl_control_map[SDLK_DOWN] = rd::RdKey::KEY_ARROW_DOWN;
        sdl_control_map[SDLK_LEFT] = rd::RdKey::KEY_ARROW_LEFT;
        sdl_control_map[SDLK_RIGHT] = rd::RdKey::KEY_ARROW_RIGHT;
        sdl_control_map[SDLK_RETURN] = rd::RdKey::KEY_ENTER;

        initialized = true;
    }

    return initialized;
}
