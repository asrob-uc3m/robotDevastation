// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __GAME_STATE_HPP__
#define __GAME_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "RdUtils.hpp"
#include "Hub.hpp"
#include "RdYarpNetworkManager.hpp"
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
* @brief Game State
* Behavior:
*  - Manages game interaction (movement, hits, player info updates)
*  - When user presses the Esc key, logs out
*  - It also plays music (important)
*/
class GameState : public State, public ManagerHub, public RdInputEventListener
{
    public:
        GameState(RdNetworkManager * networkManager, RdImageManager * imageManager,
                  RdInputManager * inputManager, RdMentalMap * mentalMap,
                  RdRobotManager * robotManager, AudioManager * audioManager);
        virtual ~GameState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

        static const int KILLED;
        static const int QUIT_REQUESTED;

        //-- RdInputEventListener interface:
        virtual bool onKeyDown(RdKey k);
        virtual bool onKeyUp(RdKey k);

    protected:
        RdScreen * screen;
        bool login;
        bool logged_in;


};


}
#endif // __GAME_STATE_HPP__
