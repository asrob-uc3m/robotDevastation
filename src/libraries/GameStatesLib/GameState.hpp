// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __GAME_STATE_HPP__
#define __GAME_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "RdUtils.hpp"
#include "Hub.hpp"
#include "RdYarpNetworkManager.hpp"
#include "GameScreen.hpp"
#include "RdProcessorImageEventListener.hpp"
#include "MockupKey.hpp"

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
class GameState : public State, public ManagerHub,
                  public RdInputEventListener, public RdImageEventListener
{
    public:
        GameState(RdNetworkManager * networkManager, RdImageManager * imageManager,
                  RdInputManager * inputManager, RdMentalMap * mentalMap,
                  RdRobotManager * robotManager, AudioManager * audioManager,
                  ScreenManager * screenManager);
        virtual ~GameState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();

        //! @brief Returns the internal variable value as condition evaluation result
        virtual int evaluateConditions();

        static const int KILLED;
        static const int EXIT_REQUESTED;

        //-- RdInputEventListener interface:
        virtual bool onKeyDown(const RdKey & k);
        virtual bool onKeyUp(const RdKey & k);

        //-- ImageEventListener interface
        virtual bool onImageArrived(RdImageManager * manager);

        //-- Define keys that trigger actions
        static const RdKey KEY_EXIT;
        static const RdKey KEY_SHOOT;
        static const RdKey KEY_RELOAD;
        static const RdKey KEY_MOVE_FWD;
        static const RdKey KEY_MOVE_BACK;
        static const RdKey KEY_TURN_LEFT;
        static const RdKey KEY_TURN_RIGHT;
        static const RdKey KEY_PAN_LEFT;
        static const RdKey KEY_PAN_RIGHT;
        static const RdKey KEY_TILT_UP;
        static const RdKey KEY_TILT_DOWN;

    protected:
        GameScreen screen;
        bool received_exit;
        RdProcessorImageEventListener processorImageEventListener;
        RdImage last_camera_frame;


};


}
#endif // __GAME_STATE_HPP__
