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
    //printf("[RateThreadOutput] run()\n");


    yarp::sig::ImageOf<yarp::sig::PixelRgb> *inYarpImg = pInImg->read(false);
    if (inYarpImg==NULL) {
        //printf("No img yet...\n");
        return;
    };
    
    if(!cameraInitialized)
    {
        cameraDepth=8;  // the depth of the surface in bits
        cameraChannels=3;  // R G B
        cameraWidthstep = inYarpImg->getRowSize(); // the length of a row of pixels in bytes 1920 for 640x480
        cameraWidth = inYarpImg->width();
        cameraHeight = inYarpImg->height();
        cameraInitialized = true;
    }

    // http://stackoverflow.com/1questions/393954/how-to-convert-an-opencv-iplimage-to-an-sdl-surface
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom( (void*)inYarpImg->getRawImage(),
                                                     cameraWidth, cameraHeight,
                                                     cameraDepth*cameraChannels,
                                                     cameraWidthstep,
                                                     0x0000ff, 0x00ff00, 0xff0000, 0
                                                   );


    //http://gameprogrammingtutorials.blogspot.com.es/2010/01/sdl-tutorial-series-part-4-how-to-load.html

    // Apply the image to the display
    if (SDL_BlitSurface(surface, NULL, display, NULL) != 0)
    {
        RD_ERROR("SDL_BlitSurface(): %s\n", SDL_GetError());
        ::exit(1);
    }

    //-- Test drawing the scope on screen
    //--------------------------------------------------------------------------------------
    //-- Load png file
    SDL_Surface * image_scope = IMG_Load("../../share/images/test_scope_small.png");
    if (!image_scope)
    {
        RD_ERROR("Image not loaded!");
        ::exit(1);
    }

    //-- Specify where to put the scope:
    SDL_Rect src, dst;
    src.x = 0; src.y = 0;
    src.h = image_scope->h; src.w = image_scope->w;

    dst.x = (640 - 300) / 2;
    dst.y = (480 - 300) / 2;
    dst.h = image_scope->h; dst.w = image_scope->w;

    SDL_BlitSurface(image_scope, &src, display, &dst);

    //-------------------------------------------------------------------------------------

    SDL_Flip(display);  // SDL_DOUBLEBUF flips with hw accel, if not just updates.

    //SDL_FreeSurface(surface); // needed? seems not from checking free.

    //SDL_Event evt;
    //SDL_WaitEvent(&evt);

}

void rd::RateThreadOutput::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}

