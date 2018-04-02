// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_I_ROBOT_MANAGER_HPP__
#define __RD_I_ROBOT_MANAGER_HPP__


namespace rd{

/**
 * @ingroup RobotLib
 *
 * @brief Base class for Robot Managers. Original copy of this file at https://github.com/asrob-uc3m/robotDevastation/blob/develop/src/libraries/RobotLib/IRobotManager.hpp
 *
 */
class IRobotManager
{
    public:
        //-- Robot movement related functions    
        virtual bool moveForward(int velocity) = 0;
        virtual bool moveBackwards(int velocity) = 0;
        virtual bool turnLeft(int velocity) = 0;
        virtual bool turnRight(int velocity) = 0;
        virtual bool stopMovement() = 0;

        //-- Robot camera related functions
        virtual bool tiltUp(int velocity) = 0;
        virtual bool tiltDown(int velocity) = 0;
        virtual bool panLeft(int velocity) = 0;
        virtual bool panRight(int velocity) = 0;
        virtual bool stopCameraMovement() = 0;
        
        //-- Robot connection related functions
        /// @brief Connect to the remote robot
        virtual bool connect() = 0;

        /// @brief Disconnect from the remote robot
        virtual bool disconnect() = 0;

        /// @brief Test connection (not in used yet)
        virtual bool test() = 0;

        /// @brief Enable/disable sending commands through the manager
        virtual void setEnabled(bool enabled) = 0;

        //-- Other
        virtual void onDestroy() = 0;
        virtual ~IRobotManager() {}
};

}

#endif //-- __RD_I_ROBOT_MANAGER_HPP__
