// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EventInput.hpp"

/*void rd::EventInput::init(yarp::os::ResourceFinder &rf)
{
    rateMs = DEFAULT_RATE_MS;

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

    this->setRate(rateMs);
    this->start();
}*/

void rd::EventInput::run()
{
    //printf("[EventInput] run()\n");

    SDL_Event event;

    while( SDL_PollEvent( &event ) )
    {
        /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
        switch( event.type )
        {
            case SDL_KEYDOWN:
                //printf( "Key press detected\n" );
                switch( event.key.keysym.sym )
                {
                    case SDLK_SPACE:
                        printf( "SPACE detected\n" );
                        break;
                    default:
                        break;
                }                
                break;

            //case SDL_KEYUP:
            //    printf( "Key release detected\n" );
            //    break;

            default:
                break;
        }
    }
}

/*
void rd::EventInput::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}
*/

