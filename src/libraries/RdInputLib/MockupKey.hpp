// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_KEY_HPP__
#define __MOCKUP_KEY_HPP__


#include "RdKey.hpp"
#include <map>


namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Class that represents a keyboard key, that can be created by the user
 *
 */
class MockupKey : public RdKey
{
    public:
        MockupKey(char printable_character);
        MockupKey(int control_key);

    private:
        MockupKey() {}
};
}
#endif // __MOCKUP_KEY_HPP__
