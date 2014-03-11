// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_HIGHGUI_HPP__
#define __RD_INPUT_HIGHGUI_HPP__

#include "RdInputBase.hpp"
#include "RdOutputHighgui.hpp"
#include "RdIniReader.hpp"

#include <pthread.h>

namespace rdlib{

/**
 * @ingroup RdInputLib
 *
 * @brief Implements a keyboard input for Robot Devastation using OpenCV's Highgui.
 */
class RdInputHighgui : public RdInputBase {
    public:

        virtual bool start();

        bool setRdOutputHighguiPtr( rdlib::RdOutputHighgui * rdOutputHighgui );

    protected:
        virtual bool input();

        //-- Add queue here (and its mutex)
        char * inputKey;
        char lastInputKey;
        pthread_mutex_t * inputKeyMutex;

        //-- Refence to Highgui output module:
        rdlib::RdOutputHighgui * rdOutputHighguiPtr;

};

} //rdlib

#endif  // __RD_INPUT_HIGHGUI_HPP__

