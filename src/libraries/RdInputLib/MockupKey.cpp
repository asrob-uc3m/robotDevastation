#include "MockupKey.hpp"

rd::MockupKey::MockupKey(char printable_character)
{
    printable = true;
    char_value = printable_character;

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
    else
        control = false;
}

rd::MockupKey::MockupKey(int control_key)
{
    control = true;
    key_value = control_key;

    if (control_key == KEY_SPACE)
    {
        printable = true;
        char_value = ' ';
    }
    else if ( control_key == KEY_ENTER)
    {
        printable = true;
        char_value = '\n';
    }
    else
        printable = false;
}
