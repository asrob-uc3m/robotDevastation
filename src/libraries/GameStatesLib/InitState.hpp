// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __INIT_STATE_HPP__
#define __INIT_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "RdUtils.hpp"
#include "Hub.hpp"
#include "RdYarpNetworkManager.hpp"
#include "ScreenManager.hpp"
#include "InitScreen.hpp"

#include <string>
#include <sstream>

#include <yarp/os/Network.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/Port.h>
#include <yarp/os/BufferedPort.h>


namespace rd{

/**
* @ingroup GameStatesLib
*
* @brief Game Initial State
* Behavior:
*  - Waits for user input (showing nice RobotDevastation screen)
*  - When user presses a button (Enter?), connects to the server and
* starts the game
*  - It also plays music (important)
*/
class InitState : public State, public ManagerHub, public RdInputEventListener
{
    public:
        //InitState();
        InitState(RdNetworkManager * networkManager, RdImageManager * imageManager,
                  RdInputManager * inputManager, RdMentalMap * mentalMap,
                  RdRobotManager * robotManager, AudioManager * audioManager,
                  ScreenManager * screenManager);
        virtual ~InitState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

        static const int LOGIN_SUCCESSFUL;

        //-- RdInputEventListener interface:
        virtual bool onKeyDown(const RdKey & k);
        virtual bool onKeyUp(const RdKey & k);

    protected:
        InitScreen screen;
        bool login;
        bool logged_in;


};


}
#endif // __INIT_STATE_HPP__
