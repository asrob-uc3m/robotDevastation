// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_KEYBOARD_HPP__
#define __RD_INPUT_KEYBOARD_HPP__

#include "RdInputBase.hpp"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

namespace rdlib{

/**
 * @ingroup RdInputLib
 *
 * @brief Implements a keyboard input for Robot Devastation.
 */
class RdInputKeyboard : public RdInputBase {
    public:
        RdInputKeyboard();
        ~RdInputKeyboard();
        virtual bool init();
        virtual bool stop();
    protected:
        bool quiet;
        Display *dis;
        Window win;
        XSetWindowAttributes at;

};

} //rdlib

#endif  // __RD_INPUT_KEYBOARD_HPP__

