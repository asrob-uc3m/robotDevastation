// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RateThreadOutput.hpp"

void rd::RateThreadOutput::init(yarp::os::ResourceFinder &rf)
{
    rateMs = DEFAULT_RATE_MS;

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

    //display = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    display = SDL_SetVideoMode(128, 128, 8, SDL_HWSURFACE | SDL_DOUBLEBUF);
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
        printf("No img yet...\n");
        return;
    };
    
    // http://stackoverflow.com/questions/393954/how-to-convert-an-opencv-iplimage-to-an-sdl-surface
    SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)inYarpImg->getIplImage(),
                inYarpImg->width(),
                inYarpImg->height(),
                inYarpImg->depth,
                3,
                0xff0000, 0x00ff00, 0x0000ff, 0
                );

    printf("[RateThreadOutput] w:%d h:%d.\n",inYarpImg->width(),
                inYarpImg->height());

    //http://gameprogrammingtutorials.blogspot.com.es/2010/01/sdl-tutorial-series-part-4-how-to-load.html

    // Apply the image to the display
    if (SDL_BlitSurface(surface, NULL, display, NULL) != 0)
    {
        //cerr << "SDL_BlitSurface() Failed: " << SDL_GetError() << endl;
        RD_ERROR("SDL_BlitSurface(): %s\n", SDL_GetError());
        ::exit(1);
    }

    //int x_pos=0, y_pos=0;
    //SDL_Rect rcDest = { x_pos, y_pos, 0, 0 };
    //SDL_BlitSurface ( image, NULL, surface, &rcDest );

    // {yarp ImageOf Rgb -> openCv Mat Bgr}
    //IplImage *inIplImage = cvCreateImage(cvSize(inYarpImg->width(), inYarpImg->height()),
    //                                     IPL_DEPTH_8U, 3 );
    //cvCvtColor((IplImage*)inYarpImg->getIplImage(), inIplImage, CV_RGB2BGR);
    //Mat inCvMat(inIplImage);


}

void rd::RateThreadOutput::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}

