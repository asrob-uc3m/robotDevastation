// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_GAME_STATE_HPP__
#define __RD_GAME_STATE_HPP__

#include "State.hpp"
#include "ManagerHub.hpp"
#include "InputEventListener.hpp"
#include "ImageEventListener.hpp"
#include "NetworkManager.hpp"
#include "ImageManager.hpp"
#include "InputManager.hpp"
#include "MentalMap.hpp"
#include "IRobotManager.hpp"
#include "AudioManager.hpp"
#include "ScreenManager.hpp"
#include "Key.hpp"
#include "WindowEvent.hpp"
#include "ProcessorImageEventListener.hpp"
#include "GameScreen.hpp"

namespace rd{

/**
* @ingroup GameStatesLib
*
* @brief Game State
*
* Behavior:
*  - Manages game interaction (movement, hits, player info updates)
*  - When user presses the Esc key, logs out
*  - It also plays music (important)
*/
class GameState : public State, public ManagerHub,
                  public InputEventListener, public ImageEventListener
{
    public:
        GameState(NetworkManager * networkManager, ImageManager * imageManager,
                  InputManager * inputManager, MentalMap * mentalMap,
                  asrob::IRobotManager * robotManager, AudioManager * audioManager,
                  ScreenManager * screenManager);
        virtual ~GameState();
        virtual bool setup();
        virtual bool loop();
        virtual bool cleanup();
        virtual int evaluateConditions();

        static const int KILLED;
        static const int EXIT_REQUESTED;

        //-- InputEventListener interface:
        virtual bool onKeyDown(const Key & k);
        virtual bool onKeyUp(const Key & k);
        virtual bool onWindowEvent(const WindowEvent & event);

        //-- ImageEventListener interface
        virtual bool onImageArrived(ImageManager * manager);

        //-- Define keys that trigger actions
        static const Key KEY_EXIT;
        static const Key KEY_SHOOT;
        static const Key KEY_RELOAD;
        static const Key KEY_MOVE_FWD;
        static const Key KEY_MOVE_BACK;
        static const Key KEY_TURN_LEFT;
        static const Key KEY_TURN_RIGHT;
        static const Key KEY_PAN_LEFT;
        static const Key KEY_PAN_RIGHT;
        static const Key KEY_TILT_UP;
        static const Key KEY_TILT_DOWN;

    protected:
        GameScreen screen;
        bool received_exit;
        ProcessorImageEventListener processorImageEventListener;
        Image last_camera_frame;


};


}
#endif // __RD_GAME_STATE_HPP__
