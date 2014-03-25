// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ROBOT_BASE_HPP__
#define __RD_ROBOT_BASE_HPP__

#include <iostream>
#include <map>

#include "RdMacros.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdRobotLib
 *
 * @brief The RdRobotLib library contains the \ref RdRobotBase class and some inherited implementations.
 */

namespace rdlib{

class RdInputBase;
class RdManagerBase;

/**
 * @ingroup RdRobotLib
 *
 * @brief An abstract base class for Robot Devastation robots ( Laser Tower Of Death...).
 *
 */
class RdRobotBase {
    public:
        virtual bool connect() = 0;

        //-- Every robot in this game shoots
        virtual bool shoot() = 0;

        virtual bool askToStop() = 0;
        virtual bool quit() = 0;

        virtual bool callFunctionByName(const std::string& cmd) = 0;

        void setRdIniMap(std::map< std::string, std::string >& rdIniMap);


    protected:
        /** Map */
        std::map< std::string, std::string > rdIniMap;
};

} //rdlib

#endif  // __RD_ROBOT_BASE_HPP__
