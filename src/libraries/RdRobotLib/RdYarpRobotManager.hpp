// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_YARP_ROBOT_MANAGER_HPP__
#define __RD_YARP_ROBOT_MANAGER_HPP__

#include <yarp/dev/all.h>
#include <yarp/os/Run.h>

#include <sstream>

#include "RdRobotManager.hpp"

namespace rd{

/**
 * @ingroup RdRobotManagerLib
 *
 * @brief The Robot Manager for the Rd1 robot.
 *
 */
class RdYarpRobotManager : public RdRobotManager
{
    public:

        RdYarpRobotManager(const std::string& robotName) : RdRobotManager(robotName) { }

        //-- Robot movement related functions    
        virtual bool moveForward(int velocity = UNUSED);
        virtual bool moveBackwards(int velocity = UNUSED);
        virtual bool turnLeft(int velocity = UNUSED);
        virtual bool turnRight(int velocity = UNUSED);
        virtual bool stopMovement();

        //-- Robot camera related functions
        virtual bool tiltUp(int velocity = UNUSED);
        virtual bool tiltDown(int velocity = UNUSED);
        virtual bool panLeft(int velocity = UNUSED);
        virtual bool panRight(int velocity = UNUSED);
        virtual bool stopCameraMovement();
        
        //-- Robot connection related functions
        virtual bool connect();
        virtual bool disconnect();
        virtual bool test();
        virtual void setEnabled(bool enabled);

        virtual void onDestroy();

    protected:

        yarp::dev::PolyDriver robotDevice;
        yarp::dev::IVelocityControl *vel;

};

}



#endif //-- __RD_YARP_ROBOT_MANAGER_HPP__
