#include "DeadState.hpp"

const int rd::DeadState::RESPAWN_SELECTED = 1;
const int rd::DeadState::EXIT_SELECTED = 2;
const int rd::DeadState::DEFAULT_RATE_MS = 100;


rd::DeadState::DeadState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager) :
                ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager, audioManager)
{
    this->state_id = "DeadState";

}

rd::DeadState::~DeadState()
{

}

bool rd::DeadState::setup()
{
    //-- Reset all variables
    elapsed_time = 0;
    timer = 10;
    received_respawn = false;
    received_exit = false;

    //-- Configure & show Robot Devastation dead screen:
    RdImage last_camera_frame = imageManager->getImage();
    screen.update(DeadScreen::PARAM_LAST_CAMERA_FRAME, last_camera_frame);
    screen.update(DeadScreen::PARAM_REMAINING_TIME, number2str(timer));
    screen.show();

    //-- Disable camera images
    imageManager->stop();

    //-- Disable input
    inputManager->removeInputEventListeners();

    //-- Change music to dead theme
    audioManager->stopMusic();
    audioManager->play("RD_DEAD");

    return true;
}

bool rd::DeadState::loop()
{
    //-- Do countdown
    if (timer > 0)
    {
        elapsed_time += DEFAULT_RATE_MS;

        if (elapsed_time == 1000)
        {
            timer--;
            elapsed_time = 0;
            screen.update(DeadScreen::PARAM_REMAINING_TIME, number2str(timer));
            screen.show();

            if (timer == 0)
            {
                //-- Enable input
                inputManager->addInputEventListener(this);
            }
        }

    }

    return true;
}

bool rd::DeadState::cleanup()
{
    return false;
}

int rd::DeadState::evaluateConditions()
{
    return false;
}

bool rd::DeadState::onKeyDown(rd::RdKey k)
{
    return false;
}

bool rd::DeadState::onKeyUp(rd::RdKey k)
{
    return false;
}
