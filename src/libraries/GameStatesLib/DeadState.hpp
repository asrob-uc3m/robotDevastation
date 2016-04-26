// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __DEAD_STATE_HPP__
#define __DEAD_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "RdUtils.hpp"
#include "Hub.hpp"
#include "RdYarpNetworkManager.hpp"
#include "DeadScreen.hpp"


namespace rd{

/**
* @ingroup GameStatesLib
*
* @brief Game Dead State
* Behavior:
*  - Waits for 10 seconds showing dead screen
*  - Then, it enables input:
*      - When user presses enter, respawns the robot and goes to game state again
*      - When user presses q, logout the game
*/
class DeadState : public State, public ManagerHub, public RdInputEventListener
{
public:
    DeadState(RdNetworkManager * networkManager, RdImageManager * imageManager,
              RdInputManager * inputManager, RdMentalMap * mentalMap,
              RdRobotManager * robotManager, AudioManager * audioManager);
    virtual ~DeadState();
    virtual bool setup();
    virtual bool loop();
    virtual bool cleanup();

    //! @brief Returns the internal variable value as condition evaluation result
    virtual int evaluateConditions();

    //enum DeadStateOption {RESPAWN_SELECTED, EXIT_SELECTED};
    static const int RESPAWN_SELECTED;
    static const int EXIT_SELECTED;

    //-- RdInputEventListener interface:
    virtual bool onKeyDown(RdKey k);
    virtual bool onKeyUp(RdKey k);

    static const int DEFAULT_RATE_MS;
    static const int MAX_HEALTH;

protected:
    DeadScreen screen;
    int last_transition; //-- Stores the transition that triggered the cleanup
    bool received_respawn;
    bool received_exit;
    int elapsed_time; //-- Time elapsed in ms
    int timer; //-- Countdown timer in s
};
}
#endif // __DEAD_STATE_HPP__
