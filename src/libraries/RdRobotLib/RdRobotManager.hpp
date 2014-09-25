// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ROBOT_MANAGER_HPP__
#define __RD_ROBOT_MANAGER_HPP__

#include "RdMacros.hpp"
#include <string>
#include <vector>
#include <map>

namespace rd{

class RdRobotManager
{
    public:
        //-- Robot movement related functions    
        virtual bool moveForward(int velocity = UNUSED) = 0;
        virtual bool moveBackwards(int velocity = UNUSED) = 0;
        virtual bool turnLeft(int velocity = UNUSED) = 0;
        virtual bool turnRight(int velocity = UNUSED) = 0;
        virtual bool stopMovement() = 0;

        //-- Robot camera related functions
        virtual bool tiltUp(int velocity = UNUSED) = 0;
        virtual bool tiltDown(int velocity = UNUSED) = 0;
        virtual bool panLeft(int velocity = UNUSED) = 0;
        virtual bool panRight(int velocity = UNUSED) = 0;
        
        //-- Robot connection related functions
        virtual bool connect() = 0;
        virtual bool disconnect() = 0;
        virtual bool test() = 0;
        virtual bool ping() = 0;

        //-- Other
        virtual void onDestroy() = 0;
        ~RdRobotManager();

        //-- Constants
        static const int UNUSED = -1;

        RdRobotManager();
};

}



#endif //-- __RD_ROBOT_MANAGER_HPP__
