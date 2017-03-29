// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INIT_STATE_HPP__
#define __RD_INIT_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "Utils.hpp"
#include "Hub.hpp"
#include "YarpNetworkManager.hpp"
#include "ScreenManager.hpp"
#include "InitScreen.hpp"
#include "WindowEvent.hpp"

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
*  - Closes if user presses Esc
*  - It also plays music (important)
*/
class InitState : public State, public ManagerHub, public InputEventListener
{
    public:
        //InitState();
        InitState(NetworkManager * networkManager, ImageManager * imageManager,
                  InputManager * inputManager, MentalMap * mentalMap,
                  RobotManager * robotManager, AudioManager * audioManager,
                  ScreenManager * screenManager);
        virtual ~InitState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

        static const int LOGIN_SUCCESSFUL;
        static const int EXIT_REQUESTED;

        //-- InputEventListener interface:
        virtual bool onKeyDown(const Key & k);
        virtual bool onKeyUp(const Key & k);
        virtual bool onWindowEvent(const WindowEvent & event);

    protected:
        InitScreen screen;
        bool login;
        bool logged_in;
        bool received_exit;


};


}
#endif // __RD_INIT_STATE_HPP__
