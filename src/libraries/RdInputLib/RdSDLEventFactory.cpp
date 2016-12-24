#include "RdSDLEventFactory.hpp"

bool rd::RdSDLEventFactory::initialized = false;
std::map<SDL_Keycode, char> rd::RdSDLEventFactory::sdl_printable_map = std::map<SDL_Keycode, char>();
std::map<SDL_Keycode, int>  rd::RdSDLEventFactory::sdl_control_map = std::map<SDL_Keycode, int>();

rd::RdSDLEventFactory::RdSDLEventFactory()
{
}

rd::RdSDLEventFactory::RdSDLEventFactory(const RdSDLEventFactory &)
{
}

rd::RdKey rd::RdSDLEventFactory::makeKey(SDL_Keycode keycode)
{
    //-- Initialize the key maps
    initLookupTables();

    if ( sdl_printable_map.find(keycode) != sdl_printable_map.end() )
    {
        return sdl_printable_map.at(keycode);
    }
    else if ( sdl_control_map.find(keycode) != sdl_control_map.end() )
    {
        return sdl_control_map.at(keycode);
    }
    else
    {
        RD_ERROR("Key not supported!\n");
        return rd::RdKey::KEY_UNKNOWN;
    }
}

bool rd::RdSDLEventFactory::initLookupTables()
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

rd::RdWindowEvent rd::RdSDLEventFactory::makeWindowEvent(SDL_WindowEvent windowEvent)
{
    switch ( windowEvent.event )
    {
    case SDL_WINDOWEVENT_CLOSE: return rd::RdWindowEvent::WINDOW_CLOSE;
    default:                    return rd::RdWindowEvent::WINDOW_UNKNOWN;
    }
}