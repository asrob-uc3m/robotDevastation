#include "RdKey.hpp"


rd::RdKey::RdKey()
{
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
