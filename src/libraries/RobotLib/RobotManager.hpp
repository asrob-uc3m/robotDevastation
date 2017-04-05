// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ROBOT_MANAGER_HPP__
#define __RD_ROBOT_MANAGER_HPP__


namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RobotLib
 *
 * @brief The RobotLib library contains the \ref RobotManager base class and derived classes.
 */

/**
 * @ingroup RobotLib
 *
 * @brief A base class for Robot Managers.
 *
 */
class RobotManager
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
        virtual ~RobotManager() {}

        //-- Constants
        static const int UNUSED = -1;

    protected:
};

}



#endif //-- __RD_ROBOT_MANAGER_HPP__
