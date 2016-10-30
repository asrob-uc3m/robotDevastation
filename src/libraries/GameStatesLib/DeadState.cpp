#include "DeadState.hpp"

const int rd::DeadState::RESPAWN_SELECTED = 1;
const int rd::DeadState::EXIT_SELECTED = 2;
const int rd::DeadState::DEFAULT_RATE_MS = 100;
const int rd::DeadState::MAX_HEALTH = 100;


rd::DeadState::DeadState(rd::RdNetworkManager *networkManager, rd::RdImageManager *imageManager,
                         rd::RdInputManager *inputManager, rd::RdMentalMap *mentalMap,
                         rd::RdRobotManager *robotManager, AudioManager *audioManager,
                         rd::ScreenManager *screenManager) :
                ManagerHub(networkManager, imageManager, inputManager, mentalMap, robotManager,
                           audioManager, screenManager)
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
    if( ! screen.init() )
        return false;
    screenManager->setCurrentScreen(&screen);

    RdImage last_camera_frame = imageManager->getImage();
    screenManager->update(DeadScreen::PARAM_LAST_CAMERA_FRAME, last_camera_frame);
    screenManager->update(DeadScreen::PARAM_REMAINING_TIME, number2str(timer));

    //-- Disable camera images
    imageManager->setEnabled(false);

    //-- Disable input
    inputManager->removeInputEventListeners();

    //-- Change music to dead theme
    audioManager->stopMusic();
    audioManager->play("RD_DEAD");

    //-- Disable robot controls
    robotManager->setEnabled(false);

    return true;
}

bool rd::DeadState::loop()
{
    //-- Get input events
    inputManager->refreshEvents();

    //-- Do countdown
    if (timer > 0)
    {
        elapsed_time += DEFAULT_RATE_MS;

        if (elapsed_time == 1000)
        {
            timer--;
            elapsed_time = 0;
            screenManager->update(DeadScreen::PARAM_REMAINING_TIME, number2str(timer));

            if (timer == 0)
            {
                //-- Enable input
                inputManager->addInputEventListener(this);
            }
        }
    }

    //-- Show graphics
    if(!screenManager->show())
        return false;

    return true;
}

bool rd::DeadState::cleanup()
{
    screen.cleanup();

    if (received_respawn)
    {
        //-- Restore things (health, enable stuff)
        mentalMap->respawn();

        //-- Remove this input listener (game will setup its own listener)
        inputManager->removeInputEventListeners();

        //-- Stop dead theme
        audioManager->stopMusic();

        return true;

    }
    else if (received_exit)
    {
        //-- Stop things to exit game (logout)
        imageManager->stop();
        inputManager->stop();
        inputManager->removeInputEventListeners();
        audioManager->stopMusic();
        audioManager->stop();
        networkManager->logout();
        networkManager->stop();
        robotManager->setEnabled(false);
        robotManager->disconnect();
        return true;
    }
    else
        return false;
}

int rd::DeadState::evaluateConditions()
{
    if (received_respawn)
        return RESPAWN_SELECTED;

    if (received_exit)
        return EXIT_SELECTED;

    return -1;
}

bool rd::DeadState::onKeyDown(rd::RdKey k)
{
    return true;
}

bool rd::DeadState::onKeyUp(rd::RdKey k)
{
    if (received_respawn || received_exit)
        return false;

    if (k.getValue() == RdKey::KEY_ENTER)
    {
        RD_DEBUG("Enter was pressed!\n");
        received_respawn = true;
        return true;
    }

    if (k.getValue() == RdKey::KEY_ESCAPE)
    {
        RD_DEBUG("Escape was pressed!\n");
        received_exit = true;
        return true;
    }

    return false;
}
