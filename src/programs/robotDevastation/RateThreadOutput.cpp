// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RateThreadOutput.hpp"

void rd::RateThreadOutput::init(yarp::os::ResourceFinder &rf)
{
    rateMs = DEFAULT_RATE_MS;
    cameraInitialized = false;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RateThreadOutput options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
    }
    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();

    printf("RateThreadOutput using rateMs: %d.\n", rateMs);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    imageManager = RdImageManager::getImageManager();

    // http://gameprogrammingtutorials.blogspot.com.es/2010/01/sdl-tutorial-series-part-3-your-first.html
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        RD_ERROR("SDL_Init(): %s\n",SDL_GetError());
        ::exit(1);
    }

    display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);  // SDL_SWSURFACE
    if (display == NULL)
    {
        RD_ERROR("SDL_SetVideoMode(): %s\n",SDL_GetError());
        ::exit(1);
    }

    // Set the title bar
    SDL_WM_SetCaption("Robot Devastation", "Robot Devastation");

    this->setRate(rateMs);
    this->start();

}

void rd::RateThreadOutput::run()
{
    //RD_DEBUG("[RateThreadOutput] run()\n");

    SDL_Surface *surface = RdImage2SDLImage(imageManager->getImage());
    if (surface == NULL)
        return;

    //http://gameprogrammingtutorials.blogspot.com.es/2010/01/sdl-tutorial-series-part-4-how-to-load.html

    // Apply the image to the display
    if (SDL_BlitSurface(surface, NULL, display, NULL) != 0)
    {
        RD_ERROR("SDL_BlitSurface(): %s\n", SDL_GetError());
        ::exit(1);
    }

    gameScreen.show(display);

    SDL_Flip(display);  // SDL_DOUBLEBUF flips with hw accel, if not just updates.

    //SDL_FreeSurface(surface); // needed? seems not from checking free.

}
