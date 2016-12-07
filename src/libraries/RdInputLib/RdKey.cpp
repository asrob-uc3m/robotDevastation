#include "RdKey.hpp"

rd::RdKey::RdKey(char printable_character)
    : char_value(printable_character), key_value(KEY_PRINTABLE), printable(true), control(false)
{
    if (printable_character == ' ')
    {
        control = true;
        key_value = KEY_SPACE;
    }
    else if (printable_character == '\n')
    {
        control = true;
        key_value = KEY_ENTER;
    }
    else if (printable_character == '\0')
    {
        printable = false;
        key_value = KEY_UNKNOWN;
    }
}

rd::RdKey::RdKey(int control_key)
    : char_value('\0'), key_value(control_key), printable(false), control(true)
{
    if (control_key == KEY_SPACE)
    {
        printable = true;
        char_value = ' ';
    }
    else if (control_key == KEY_ENTER)
    {
        printable = true;
        char_value = '\n';
    }
    else if (control_key == KEY_UNKNOWN)
    {
        control = false;
    }
}

bool rd::RdKey::isControlKey() const
{
    return control;
}

bool rd::RdKey::isPrintable() const
{
    return printable;
}

char rd::RdKey::getChar() const
{
    return char_value;
}

int rd::RdKey::getValue() const
{
    return key_value;
}

rd::RdKey::~RdKey()
{
}

bool rd::RdKey::operator==(const rd::RdKey &k) const
{
    if (this->isPrintable() && k.isPrintable())
        return this->char_value == k.char_value;
    if (this->isControlKey() && k.isControlKey())
        return this->key_value == k.key_value;
    return false;
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
const int rd::RdKey::KEY_ENTER = 8;
