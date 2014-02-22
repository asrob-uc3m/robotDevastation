// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdOutputHighgui.hpp"

rdlib::RdOutputHighgui::RdOutputHighgui()
{
    std::cout << "[info] RdOutputHighgui::RdOutputHighgui()"<< std::endl;

    //-- just for testing
    videoFrame = cv::imread("../share/rdClient/graphics/cabin_ecro.png");
    if(videoFrame.empty()) {
        std::cerr << "[warning] Image not found." << std::endl;
    }

    //-- Start the output thread
    pthread_create( &output_thread, NULL, outputThread, (void *) this );
    std::cout << "[info] RdOutputHighgui created thread." << std::endl;
}

bool rdlib::RdOutputHighgui::quit()
{
    std::cout << "[info] RdOutputHighgui quit()" << std::endl;
    pthread_join( output_thread, NULL);
}

void * rdlib::RdOutputHighgui::outputThread(void *This)
{
    ( (rdlib::RdOutputHighgui *) This)->output();
}

void rdlib::RdOutputHighgui::output( )
{
    std::cout << "[info] RdOutputHighgui thread." << std::endl;
    while (!rdManagerBasePtr)
    {
        std::cout << "[info] RdOutputHighgui waiting for manager..." << std::endl;
        usleep(100000);
    }
    do
    {
        rdCameraBasePtr = rdManagerBasePtr->getRdCameraBasePtr();
        std::cout << "[info] RdOutputHighgui waiting for camera..." << std::endl;
        usleep(100000);
    }
    while ( ! (rdCameraBasePtr) );

    std::cout << "[info] RdOutputHighgui started." << std::endl;

    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    std::cout << "[info] RdOutputHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    while(1) {
        std::cout << "[info] RdOutputHighgui alive..." << std::endl;
        usleep( 500000.0 );
    }

}


