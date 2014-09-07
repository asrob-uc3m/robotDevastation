#include "RdKey.hpp"


char rd::RdKey::getChar()
{
    if ( sdl_map.find(key_value) == sdl_map.end() )
    {
        RD_ERROR("Unknown key\n");
        return '\0';
    }
    else
    {
        return sdl_map.at(key_value);
    }
}

bool rd::RdKey::setFromKeyCode(SDLKey keycode)
{
    key_value = keycode;

    if ( sdl_map.find(key_value) == sdl_map.end())
    {
        RD_WARNING("Key not supported!\n");
        return false;
    }
    else
        return true;
}

const std::map< SDLKey, char> rd::RdKey::sdl_map = { {SDLK_0, '0'},
                                                     {SDLK_1, '1'},
                                                     {SDLK_2, '2'},
                                                     {SDLK_3, '3'},
                                                     {SDLK_4, '4'},
                                                     {SDLK_5, '5'},
                                                     {SDLK_6, '6'},
                                                     {SDLK_7, '7'},
                                                     {SDLK_8, '8'},
                                                     {SDLK_9, '9'},
                                                     {SDLK_a, 'a'},
                                                     {SDLK_b, 'b'},
                                                     {SDLK_c, 'c'},
                                                     {SDLK_d, 'd'},
                                                     {SDLK_e, 'e'},
                                                     {SDLK_f, 'f'},
                                                     {SDLK_g, 'g'},
                                                     {SDLK_h, 'h'},
                                                     {SDLK_i, 'i'},
                                                     {SDLK_j, 'j'},
                                                     {SDLK_k, 'k'},
                                                     {SDLK_l, 'l'},
                                                     {SDLK_m, 'm'},
                                                     {SDLK_n, 'n'},
                                                     {SDLK_o, 'o'},
                                                     {SDLK_p, 'p'},
                                                     {SDLK_q, 'q'},
                                                     {SDLK_r, 'r'},
                                                     {SDLK_s, 's'},
                                                     {SDLK_t, 't'},
                                                     {SDLK_u, 'u'},
                                                     {SDLK_v, 'v'},
                                                     {SDLK_w, 'w'},
                                                     {SDLK_x, 'x'},
                                                     {SDLK_y, 'y'},
                                                     {SDLK_z, 'z'},
                                                   };
