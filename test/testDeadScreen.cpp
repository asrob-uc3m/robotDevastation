#include <SDL/SDL.h>
#include "SDLUtils.hpp"
#include <yarp/sig/all.h>

#include "DeadScreen.hpp"
#include "RdUtils.hpp"

using namespace rd;

int main()
{
    initSDL();

    DeadScreen screen;
    screen.init();

    //-- Load test image
    RdImage test_frame;
    yarp::sig::file::read(test_frame, "../../share/images/test_frame.ppm");
    screen.update(DeadScreen::PARAM_LAST_CAMERA_FRAME, test_frame);

    //-- Setup elapsed time
    screen.update(DeadScreen::PARAM_REMAINING_TIME, "10");

    for (int i = 0; i < 10; i++)
    {
        screen.update(DeadScreen::PARAM_REMAINING_TIME, number2str(10-i));
        screen.show();
        SDL_Delay(1000); //Wait a bit :)
    }

    screen.cleanup();

    cleanupSDL();
}
