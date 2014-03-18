// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_BASE_HPP__
#define __RD_INPUT_BASE_HPP__

#include <iostream>
#include <map>

#include "RdMacros.hpp"
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
        virtual bool start();

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit();

        void setRdManagerBasePtr(RdManagerBase* rdManagerBasePtr );


        const char& getKeyCharByName(std::string keychar_name);

        void setKeyFunctionMap(const std::map<char, void *> &value);

protected:
        //-- Thread-related
        pthread_t threadId;
        bool isRunning;

        //-- Pointers to other modules
        RdManagerBase* rdManagerBasePtr;

        virtual bool input() = 0;

        /** Map on input base to describe mapping of implementation of keys */
        std::map< std::string, char > keyMap;

        /** Map of keys to functions */
        std::map< char, void*> keyFunctionMap;

    private:
        static void * inputThread( void * This );
};

} //rdlib

#endif  // __RD_INPUT_BASE_HPP__

