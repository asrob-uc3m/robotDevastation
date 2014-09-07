#include "RdKey.hpp"


rd::RdKey::RdKey()
{
    char_value = '\0';
    key_value = KEY_UNKNOWN;
    printable = false;
    control = false;
}

bool rd::RdKey::setFromKeyCode(SDLKey keycode)
{
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
        RD_ERROR("Key not supported!\n");
        return false;
    }
    else
    {
        return true;
    }
}

bool rd::RdKey::isControlKey()
{
    return control;
}

bool rd::RdKey::isPrintable()
{
    return printable;
}

char rd::RdKey::getChar()
{
    return char_value;
}

int rd::RdKey::getValue()
{
    return key_value;
}


const int rd::RdKey::KEY_UNKNOWN = -1;
const int rd::RdKey::KEY_PRINTABLE = 0;
const int rd::RdKey::KEY_SPACE = 1;
const int rd::RdKey::KEY_ESCAPE = 2;
const int rd::RdKey::KEY_BACKSPACE = 3;
const int rd::RdKey::KEY_ARROW_UP = 4;
const int rd::RdKey::KEY_ARROW_DOWN = 5;
const int rd::RdKey::KEY_ARROW_LEFT = 6;
const int rd::RdKey::KEY_ARROW_RIGHT = 7;

const std::map< SDLKey, char> rd::RdKey::sdl_printable_map = { {SDLK_0, '0'}, {SDLK_1, '1'}, {SDLK_2, '2'}, {SDLK_3, '3'},
                                                               {SDLK_4, '4'}, {SDLK_5, '5'}, {SDLK_6, '6'}, {SDLK_7, '7'},
                                                               {SDLK_8, '8'}, {SDLK_9, '9'},

                                                               {SDLK_a, 'a'}, {SDLK_b, 'b'}, {SDLK_c, 'c'}, {SDLK_d, 'd'},
                                                               {SDLK_e, 'e'}, {SDLK_f, 'f'}, {SDLK_g, 'g'}, {SDLK_h, 'h'},
                                                               {SDLK_i, 'i'}, {SDLK_j, 'j'}, {SDLK_k, 'k'}, {SDLK_l, 'l'},
                                                               {SDLK_m, 'm'}, {SDLK_n, 'n'}, {SDLK_o, 'o'}, {SDLK_p, 'p'},
                                                               {SDLK_q, 'q'}, {SDLK_r, 'r'}, {SDLK_s, 's'}, {SDLK_t, 't'},
                                                               {SDLK_u, 'u'}, {SDLK_v, 'v'}, {SDLK_w, 'w'}, {SDLK_x, 'x'},
                                                               {SDLK_y, 'y'}, {SDLK_z, 'z'},

                                                               {SDLK_SPACE, ' '},
                                                             };

const std::map< SDLKey, int> rd::RdKey::sdl_control_map = { {SDLK_SPACE, rd::RdKey::KEY_SPACE},
                                                             {SDLK_ESCAPE, rd::RdKey::KEY_ESCAPE},
                                                             {SDLK_BACKSPACE, rd::RdKey::KEY_BACKSPACE},
                                                             {SDLK_UP, rd::RdKey::KEY_ARROW_UP},
                                                             {SDLK_DOWN, rd::RdKey::KEY_ARROW_DOWN},
                                                             {SDLK_LEFT, rd::RdKey::KEY_ARROW_LEFT},
                                                             {SDLK_RIGHT, rd::RdKey::KEY_ARROW_RIGHT}
                                                           };
