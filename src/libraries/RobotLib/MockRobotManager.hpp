// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MOCK_ROBOT_MANAGER_HPP__
#define __RD_MOCK_ROBOT_MANAGER_HPP__

#include <string>

#include "IRobotManager.hpp"

namespace rd{

/**
 * @ingroup RobotLib
 *
 * @brief The Robot Manager for testing
 *
 */
class MockRobotManager : public RobotManager
{
    public:

        MockRobotManager(const std::string& robotName);

        //-- RobotManager interface
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

        //-- Mock functionality
        //-----------------------------------------------------
        bool isConnected() const;
        bool isEnabled() const;

        bool isMoving() const;
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
        std::string robotName;
        bool enabled;
        bool connected;

        int movement_direction;
        int camera_movement_direction;

};

}



#endif //-- __RD_MOCK_ROBOT_MANAGER_HPP__
