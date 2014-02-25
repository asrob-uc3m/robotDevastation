// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_BASE_HPP__
#define __RD_MANAGER_BASE_HPP__

#include <iostream>
#include "RdInputBase.hpp"
#include "RdRobotBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdManagerLib
 *
 * @brief The RdManagerLib library contains the \ref RdManagerBase class and some inherited implementations.
 */

namespace rdlib{

class RdCameraBase;
class RdInputBase;
class RdOutputBase;
class RdRobotBase;

/**
 * @ingroup RdManagerLib
 *
 * @brief An abstract base class for Robot Devastation managers.
 *
 */
class RdManagerBase {
    public:

        RdManagerBase() {
            rdCameraBasePtr = 0;
            rdInputBasePtr = 0;
            rdOutputBasePtr = 0;
            rdRobotBasePtr = 0;

            //-- Add shoot to dictionary
            functionMap[ "shoot"] = (void *) &shootWrapper;
        }

        virtual bool start() = 0;

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit() = 0;

        /** An shoot rutine.
         * @return true if successful.
         */
        virtual bool shoot() = 0;

        static bool shootWrapper(void *This)
        {
            return (( rdlib::RdManagerBase *) This)->shoot();
        }

        void *getFunctionByName( std::string function_name ) {
            return functionMap[ function_name ];
        }

        int getManagerStatus() {
            return managerStatus;
        }

        void setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr ) {
            this->rdCameraBasePtr = rdCameraBasePtr;
        }
        void setRdInputBasePtr(RdInputBase* rdInputBasePtr ) {
            this->rdInputBasePtr = rdInputBasePtr;
        }
        void setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr ) {
            this->rdOutputBasePtr = rdOutputBasePtr;
        }
        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr ) {
            this->rdRobotBasePtr = rdRobotBasePtr;
        }

        RdCameraBase* getRdCameraBasePtr() {
            return this->rdCameraBasePtr;
        }

    protected:
        RdCameraBase* rdCameraBasePtr;
        RdInputBase* rdInputBasePtr;
        RdOutputBase* rdOutputBasePtr;
        RdRobotBase* rdRobotBasePtr;
        int managerStatus;
        std::map< std::string, void*> functionMap;
};

} //rdlib

#endif  // __RD_MANAGER_BASE_HPP__

