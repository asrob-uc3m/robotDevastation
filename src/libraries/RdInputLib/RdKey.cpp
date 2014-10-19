#include "RdKey.hpp"


rd::RdKey::RdKey()
{
    char_value = '\0';
    key_value = KEY_UNKNOWN;
    printable = false;
    control = false;
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

	sdl_control_map[SDLK_SPACE] = rd::RdKey::KEY_SPACE;
    sdl_control_map[SDLK_ESCAPE] = rd::RdKey::KEY_ESCAPE;
    sdl_control_map[SDLK_BACKSPACE] = rd::RdKey::KEY_BACKSPACE;
    sdl_control_map[SDLK_UP] = rd::RdKey::KEY_ARROW_UP;
    sdl_control_map[SDLK_DOWN] = rd::RdKey::KEY_ARROW_DOWN;
    sdl_control_map[SDLK_LEFT] = rd::RdKey::KEY_ARROW_LEFT;
    sdl_control_map[SDLK_RIGHT] = rd::RdKey::KEY_ARROW_RIGHT;
  
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

/*const std::map< SDLKey, char> rd::RdKey::sdl_printable_map = { {SDLK_0, '0'}, {SDLK_1, '1'}, {SDLK_2, '2'}, {SDLK_3, '3'},
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
                                                             };*/

/*const std::map< SDLKey, int> rd::RdKey::sdl_control_map = {  {SDLK_SPACE, rd::RdKey::KEY_SPACE},
                                                             {SDLK_ESCAPE, rd::RdKey::KEY_ESCAPE},
                                                             {SDLK_BACKSPACE, rd::RdKey::KEY_BACKSPACE},
                                                             {SDLK_UP, rd::RdKey::KEY_ARROW_UP},
                                                             {SDLK_DOWN, rd::RdKey::KEY_ARROW_DOWN},
                                                             {SDLK_LEFT, rd::RdKey::KEY_ARROW_LEFT},
                                                             {SDLK_RIGHT, rd::RdKey::KEY_ARROW_RIGHT}
                                                           };*/
