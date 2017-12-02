// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_DEAD_STATE_HPP__
#define __RD_DEAD_STATE_HPP__

#include "State.hpp"
#include "Hub.hpp"
#include "InputEventListener.hpp"
#include "NetworkManager.hpp"
#include "ImageManager.hpp"
#include "InputManager.hpp"
#include "MentalMap.hpp"
#include "IRobotManager.hpp"
#include "AudioManager.hpp"
#include "ScreenManager.hpp"
#include "DeadScreen.hpp"
#include "Key.hpp"
#include "WindowEvent.hpp"

namespace rd{

/**
* @ingroup GameStatesLib
*
* @brief Game Dead State
*
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
              IRobotManager * robotManager, AudioManager * audioManager,
              ScreenManager * screenManager);
    virtual ~DeadState();
    virtual bool setup();
    virtual bool loop();
    virtual bool cleanup();
    virtual int evaluateConditions();

    //enum DeadStateOption {RESPAWN_SELECTED, EXIT_SELECTED};
    static const int RESPAWN_SELECTED;
    static const int EXIT_SELECTED;

    //-- InputEventListener interface:
    virtual bool onKeyDown(const Key & k);
    virtual bool onKeyUp(const Key & k);
    virtual bool onWindowEvent(const WindowEvent & event);

    static const int DEFAULT_RATE_MS;
    static const int MAX_HEALTH;

protected:
    DeadScreen screen;
    bool received_respawn;
    bool received_exit;
    int elapsed_time; //-- Time elapsed in ms
    int timer; //-- Countdown timer in s
};
}
#endif // __RD_DEAD_STATE_HPP__
