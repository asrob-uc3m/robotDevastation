// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdInOutHighgui.hpp"

rdlib::RdInOutHighgui::RdInOutHighgui()
{
    std::cout << "[info] RdInOutHighgui::RdInOutHighgui()"<< std::endl;

    //-- just for testing
    videoFrame = cv::imread("../share/rdClient/graphics/cabin_ecro.png");
    if(videoFrame.empty()) {
        std::cerr << "[warning] Image not found." << std::endl;
    }

    //-- Start the output thread
    pthread_create( &output_thread, NULL, outputThread, (void *) this );
    std::cout << "[info] RdInOutHighgui created thread." << std::endl;
}

bool rdlib::RdInOutHighgui::quit()
{
    std::cout << "[info] RdInOutHighgui quit()" << std::endl;
    pthread_join( output_thread, NULL);
}

void * rdlib::RdInOutHighgui::outputThread(void *This)
{
    ( (rdlib::RdInOutHighgui *) This)->output();
}

void rdlib::RdInOutHighgui::output( )
{
    std::cout << "[info] RdInOutHighgui thread." << std::endl;
    while (!rdManagerBasePtr)
    {
        std::cout << "[info] RdInOutHighgui waiting for manager..." << std::endl;
        usleep(100000);
    }
    do
    {
        rdCameraBasePtr = rdManagerBasePtr->getRdCameraBasePtr();
        std::cout << "[info] RdInOutHighgui waiting for camera..." << std::endl;
        usleep(100000);
    }
    while ( ! (rdCameraBasePtr) );

    std::cout << "[info] RdInOutHighgui started." << std::endl;

    int width,height,step;
    rdCameraBasePtr->getDimensions(width,height,step);
    std::cout << "[info] RdInOutHighgui got camera [w:" << width << ", h:" << height << ", s:" << step  << "]" << std::endl;

    cv::namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
    cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    while(1) {
        //std::cout << "[info] RdInOutHighgui alive..." << std::endl;
        //j//char * bufferPtr = rdCameraBasePtr->getBufferPtr();
        //j//char bufferPtr[width* height];
        //j//bufferPtr = rdCameraBasePtr->getBufferPtr();
        cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(), step);  // cv::Mat::AUTO_STEP ???
        cv::imshow("Robot Devastation", image);  // no cv:: needed.
        char c = cv::waitKey( 1 );  // [ms]
        if (c == ' ')  {  // SPACE: 1048608 (0x100020), LSB: 32 (' ')
            std::cout << "The space bar was pressed." << std::endl;
            rdManagerBasePtr->shoot();
        } else if ( c == '\x1b' ) {  // ESC: 1048603 (0x10001b), LSB: 27 ('\x1b')
            std::cout << "The escape key was pressed. Bye!" << std::endl;
            rdManagerBasePtr->quit();
        }
    }
}


