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
class MockRobotManager : public asrob::IRobotManager
{
    public:

        MockRobotManager(const std::string& robotName);

        //-- RobotManager interface
        //-----------------------------------------------------
        /// @brief Robot: Move forward (use negative value for move backward). position mode [m]. Velocity mode [m/s].
        virtual bool moveForward(double value);

        /// @brief Robot: Turn left (use negative value for turn left). position mode [deg]. Velocity mode [deg/s].
        virtual bool turnLeft(double value);

        /// @brief Robot: Stop movement.
        virtual bool stopMovement();

        /// @brief Robot camera: Tilt down (use negative value for tilt up). position mode [deg]. Velocity mode [deg/s].
        virtual bool tiltDown(double value);

        /// @brief Robot camera: Pan left (use negative value for pan right). position mode [deg]. Velocity mode [deg/s].
        virtual bool panLeft(double value);

        /// @brief Robot camera: Stop movement.
        virtual bool stopCameraMovement();

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
