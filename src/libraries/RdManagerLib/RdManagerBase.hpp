// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MANAGER_BASE_HPP__
#define __RD_MANAGER_BASE_HPP__

#include <iostream>
#include "RdInputBase.hpp"
#include "RdRobotBase.hpp"
#include "RdCameraBase.hpp"

#include <vector>
#include <utility>

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

        RdManagerBase();

        virtual bool start() = 0;

        /** A quit rutine.
         * @return true if the object was quit successfully.
         */
        virtual bool quit() = 0;

        /** An shoot rutine.
         * @return true if successful.
         */
        virtual bool shoot() = 0;
        static bool shootWrapper(void *This);

        void *getFunctionByName( std::string function_name );
        int getManagerStatus();
        RdCameraBase* getRdCameraBasePtr();
        void getEnemies( int index,  std::vector< std::pair<int, int> >& enemyPos, std::vector< double >& enemySize);

        void setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr );
        void setRdInputBasePtr(RdInputBase* rdInputBasePtr ) ;
        void setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr );
        void setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr );

    protected:
        RdCameraBase* rdCameraBasePtr;
        RdInputBase* rdInputBasePtr;
        RdOutputBase* rdOutputBasePtr;
        RdRobotBase* rdRobotBasePtr;
        int managerStatus;
        std::map< std::string, void*> functionMap;

        //-- Enemies
        std::vector< std::vector< std::pair<int, int> > > enemyPos;
        std::vector< std::vector< double > > enemySize;
};

} //rdlib

#endif  // __RD_MANAGER_BASE_HPP__

