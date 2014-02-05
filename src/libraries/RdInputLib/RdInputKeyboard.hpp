// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_KEYBOARD_HPP__
#define __RD_INPUT_KEYBOARD_HPP__

#include "RdInputBase.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * The RdInputKeyboard class implements keyboard input.
 * 
 */
class RdInputKeyboard : RdInputBase {
    public:
        virtual bool init();
        virtual bool stop();
    protected:
        bool quiet;
};

} //rd

#endif  // __RD_INPUT_KEYBOARD_HPP__

