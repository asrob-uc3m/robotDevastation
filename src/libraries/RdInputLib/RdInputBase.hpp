// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_BASE_HPP__
#define __RD_INPUT_BASE_HPP__

#include <iostream>
#include "RdManagerBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputLib
 *
 * @brief The RdInputLib library contains the \ref RdInputBase class and some inherited implementations.
 */

namespace rdlib{

class RdManagerBase;
class RdRobotBase;

/**
 * @ingroup RdInputLib
 *
 * @brief An abstract base class for Robot Devastation inputs (keyboard, xpad...).
 *
 */
class RdInputBase {
    public:
        virtual bool setup();
        virtual bool start();
        virtual bool askToStop();

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit();

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr );

    protected:
        //-- Thread-related
        pthread_t threadId;
        bool stopThread;

        //-- Pointers to other modules
        RdManagerBase* rdManagerBasePtr;

        virtual bool input() = 0;

    private:
        static void * inputThread( void * This );
};

} //rdlib

#endif  // __RD_INPUT_BASE_HPP__

