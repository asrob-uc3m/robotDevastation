#include <SDL/SDL.h>
#include "SDLUtils.hpp"
#include <yarp/sig/all.h>
#include <string>
#include <vector>

#include "RdMentalMap.hpp"
#include "RdTarget.hpp"
#include "RdPlayer.hpp"
#include "RdWeapon.hpp"
#include "GameScreen.hpp"

using namespace rd;

int main(void)
{
    initSDL();

    GameScreen screen;
    screen.init();

    //-- Create a mental map with some info:
    RdMentalMap * mentalMap = RdMentalMap::getMentalMap();
    mentalMap->configure(0);

    std::vector<RdPlayer> players;
    players.push_back(RdPlayer(0, "Myself", 50, 100, 0, 0));
    players.push_back(RdPlayer(1, "Enemy1", 50, 100, 1, 0));
    players.push_back(RdPlayer(2, "Enemy2", 75, 100, 1, 0));
    mentalMap->updatePlayers(players);

    std::vector<RdTarget> targets;
    targets.push_back(RdTarget(1, RdVector2d(100, 100), RdVector2d(50, 50)));
    targets.push_back(RdTarget(2, RdVector2d(400, 200), RdVector2d(100, 100)));
    targets.push_back(RdTarget());
    mentalMap->updateTargets(targets);

    mentalMap->addWeapon(RdWeapon("Machine gun", 10, 250));

    //-- Set info elements on GameScreen
    screen.update(GameScreen::PARAM_MYSELF, mentalMap->getMyself());
    screen.update(GameScreen::PARAM_PLAYERS, mentalMap->getPlayers());
    screen.update(GameScreen::PARAM_TARGETS, mentalMap->getTargets());
    screen.update(GameScreen::PARAM_WEAPON, mentalMap->getCurrentWeapon());

    //-- Load test image
    RdImage frame;
    yarp::sig::file::read(frame, "../../share/images/test_frame.ppm");
    screen.update(GameScreen::PARAM_CAMERA_FRAME, frame);

    for (int i = 0; i < 200; i++)
    {
        screen.show();
        SDL_Delay(20); //Wait a bit :)
    }

    RdMentalMap::destroyMentalMap();
    screen.cleanup();
    cleanupSDL();
}



