// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EventInput.hpp"

/*void rd::EventInput::init(yarp::os::ResourceFinder &rf)
{
    rateMs = DEFAULT_RATE_MS;
    cameraInitialized = false;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("EventInput options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
    }
    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();

    printf("EventInput using rateMs: %d.\n", rateMs);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

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

}*/

void rd::EventInput::run()
{
    printf("[EventInput] run()\n");

    //SDL_FreeSurface(surface); // needed? seems not from checking free.

    SDL_Event evt;
    SDL_WaitEvent(&evt);

}

/*
void rd::EventInput::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}
*/

