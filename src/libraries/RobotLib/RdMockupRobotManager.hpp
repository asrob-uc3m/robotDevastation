// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MOCKUP_ROBOT_MANAGER_HPP__
#define __RD_MOCKUP_ROBOT_MANAGER_HPP__


#include <sstream>

#include "RdRobotManager.hpp"

#include "RdMacros.hpp"

namespace rd{

/**
 * @ingroup RdRobotManagerLib
 *
 * @brief The Robot Manager for testing
 *
 */
class RdMockupRobotManager : public RdRobotManager
{
    public:

        RdMockupRobotManager(const std::string& robotName);

        //-- RdRobotManager interface
        //-----------------------------------------------------
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

        //-- Mockup functionality
        //-----------------------------------------------------
        bool isConnected();
        bool isEnabled();

        bool isMoving();
        int getMovementDirection();
        static const int FORWARD;
        static const int BACKWARDS;
        static const int LEFT;
        static const int RIGHT;
        static const int NONE;

        bool isCameraMoving();
        int getCameraMovementDirection();
        static const int CAMERA_UP;
        static const int CAMERA_DOWN;
        static const int CAMERA_LEFT;
        static const int CAMERA_RIGHT;
        static const int CAMERA_NONE;

    private:
        int movement_direction;
        int camera_movement_direction;
};

}



#endif //-- __RD_MOCKUP_ROBOT_MANAGER_HPP__
