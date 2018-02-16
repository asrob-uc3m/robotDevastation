// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_INIT_STATE_HPP__
#define __RD_INIT_STATE_HPP__

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
#include "Key.hpp"
#include "WindowEvent.hpp"
#include "InitScreen.hpp"

namespace rd{

/**
* @ingroup GameStatesLib
*
* @brief Game Initial State
*
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
                  asrob::IRobotManager * robotManager, AudioManager * audioManager,
                  ScreenManager * screenManager);
        virtual ~InitState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();
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
