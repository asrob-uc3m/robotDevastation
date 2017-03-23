// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __DEAD_STATE_HPP__
#define __DEAD_STATE_HPP__

#include "State.hpp"
#include "StateDirector.hpp"
#include "Utils.hpp"
#include "Hub.hpp"
#include "YarpNetworkManager.hpp"
#include "DeadScreen.hpp"
#include "WindowEvent.hpp"


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
class DeadState : public State, public ManagerHub, public InputEventListener
{
public:
    DeadState(NetworkManager * networkManager, ImageManager * imageManager,
              InputManager * inputManager, MentalMap * mentalMap,
              RobotManager * robotManager, AudioManager * audioManager,
              ScreenManager * screenManager);
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
    virtual bool onKeyDown(const Key & k);
    virtual bool onKeyUp(const Key & k);
    virtual bool onWindowEvent(const WindowEvent & event);

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
