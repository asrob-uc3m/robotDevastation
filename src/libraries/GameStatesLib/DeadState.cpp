#include "DeadState.hpp"

const int rd::DeadState::RESPAWN_SELECTED = 1;
const int rd::DeadState::EXIT_SELECTED = 2;
const int rd::DeadState::DEFAULT_RATE_MS = 100;
const int rd::DeadState::MAX_HEALTH = 100;


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
    if (received_respawn)
    {
        //-- Restore things (health, enable stuff)
        //-- Note: this is a workaround because one cannot modify player's health through
        //-- mentalMap directly (yet)
        std::vector<RdPlayer> players = mentalMap->getPlayers();
        int id = mentalMap->getMyself().getId();
        for (int i = 0; i < players.size(); i++)
            if (players[i].getId() == id)
            {
                players[i].setHealth(MAX_HEALTH);
                break;
            }
        mentalMap->updatePlayers(players);

        //-- Enable camera images
        imageManager->start();

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
        networkManager->logout(mentalMap->getMyself()); //-- This is kind of weird, but it is supposed to be done like this
        networkManager->stop();
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
