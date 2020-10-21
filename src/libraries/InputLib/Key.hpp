// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_KEY_HPP__
#define __RD_KEY_HPP__

namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Class that represents a keyboard key
 *
 * Keys are classified in two groups: printable keys, which are keys that have a char representation,
 * such as the 'a' key or the '7' key, and control keys, which are keys that perform an action, such
 * as the arrow keys, the escape key or the backspace key. The space case is a special case, because
 * it is considered both printable and control key.
 *
 * This class must not be inherited from. Use factory classes and functions to perform custom
 * initialization (see rd::SDLEventFactory).
 */
class Key
{
    public:
        Key(char printable_character);
        Key(int control_key);

        bool isControlKey() const;
        bool isPrintable() const;

        char getChar() const;
        int  getValue() const;

        //-- Constants for control keys (non-printable)
        //! @brief Constant representing a non-supported / unknown key
        static const int KEY_UNKNOWN;
        //! @brief Constant representing any printable key
        static const int KEY_PRINTABLE;
        static const int KEY_SPACE;
        static const int KEY_ESCAPE;
        static const int KEY_BACKSPACE;
        static const int KEY_ARROW_UP;
        static const int KEY_ARROW_DOWN;
        static const int KEY_ARROW_LEFT;
        static const int KEY_ARROW_RIGHT;
        static const int KEY_ENTER;

        bool operator==(Key const& k) const;

    protected:
        //! @brief Stores the char representation of a key
        char char_value;
        //! @brief Stores the control key value of a key
        int key_value;

        //! @brief Stores whether the key is printable or not
        bool printable;
        //! @brief Stores whether the key is a control key or not
        bool control;
};

}

#endif //-- __RD_KEY_HPP__
