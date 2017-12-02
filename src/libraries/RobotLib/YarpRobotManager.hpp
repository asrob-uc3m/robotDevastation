// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_YARP_ROBOT_MANAGER_HPP__
#define __RD_YARP_ROBOT_MANAGER_HPP__

#include <string>

#include <yarp/os/RpcClient.h>
#include <yarp/dev/all.h> // VOCAB macros

#include "IRobotManager.hpp"

#define VOCAB_MOVE_FORWARD VOCAB4('m','o','v','f')
#define VOCAB_MOVE_BACKWARDS VOCAB4('m','o','v','b')
#define VOCAB_TURN_LEFT VOCAB4('t','r','n','l')
#define VOCAB_TURN_RIGHT VOCAB4('t','r','n','r')
#define VOCAB_STOP_MOVEMENT VOCAB4('s','t','p','m')

#define VOCAB_TILT_UP VOCAB4('t','l','t','u')
#define VOCAB_TILT_DOWN VOCAB4('t','l','t','d')
#define VOCAB_PAN_LEFT VOCAB4('p','a','n','l')
#define VOCAB_PAN_RIGHT VOCAB4('p','a','n','r')
#define VOCAB_STOP_CAMERA_MOVEMENT VOCAB4('s','t','p','c')

namespace rd{

/**
 * @ingroup RobotLib
 *
 * @brief The Robot Manager for the Rd1 robot.
 *
 */
class YarpRobotManager : public RobotManager
{
    public:

        YarpRobotManager(const std::string& robotName);

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

        yarp::os::RpcClient rpcClient;

    private:
        std::string robotName;
        bool enabled;
        bool connected;

        bool send1vocab1int(int vocab, int integer);
        bool send1vocab(int vocab);

};

}



#endif //-- __RD_YARP_ROBOT_MANAGER_HPP__
