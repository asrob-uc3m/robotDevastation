// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdInOutHighgui.hpp"
#include <stdio.h>
rdlib::RdInOutHighgui::RdInOutHighgui()
{
    std::cout << "[info] RdInOutHighgui::RdInOutHighgui()"<< std::endl;

    isRunning=true;
}

bool rdlib::RdInOutHighgui::start()
{
    //-- Start the output thread
    pthread_create( &highgui_thread, NULL, highguiThread, (void *) this );
    std::cout << "[info] RdInOutHighgui created thread." << std::endl;
}

bool rdlib::RdInOutHighgui::quit()
{
    std::cout << "[info] RdInOutHighgui quit()" << std::endl;
    isRunning=false;
    pthread_join( highgui_thread, NULL);
    return true;
}

void * rdlib::RdInOutHighgui::highguiThread(void *This)
{
    ( (rdlib::RdInOutHighgui *) This)->output();
}

void rdlib::RdInOutHighgui::output()
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
   // cv::setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

    while(isRunning) {
        for (int i = 0; i < 3; i++)
        {
            //-- Lock semaphore:
            sem_wait( displaySemaphores+i );

            //-- Display things:
            //std::cout << "[info] RdInOutHighgui alive..." << std::endl;
            cv::Mat image(cv::Size(width, height), CV_8UC3, rdCameraBasePtr->getBufferPtr(i), step);  // cv::Mat::AUTO_STEP ???
            cv::imshow("Robot Devastation", image);  // no cv:: needed.
            image.release();

            //std::cout << "[info] Displayed frame # " << i << "." << std::endl;

            //-- Unlock capture semaphore:
            sem_post( captureSemaphores+i);

            // waitKey returns key pressed, or -1 if nothing has been pressed.
            char c = cv::waitKey( 1 );  // 1 is [ms]
            if (c == ' ')   // SPACE: 1048608 (0x100020), LSB: 32 (' ')
            {
                std::cout << "[info] RdInOutHighgui: The space bar was pressed." << std::endl;
                rdManagerBasePtr->shoot();
            }
            else if ( c == 't' ) // Letter T
            {
                std::cout << "[info] RdInOutHighgui: The key 't' was pressed." << std::endl;
                //-- This enables/disables the head tracking thing
                bool (* toggleHeadTrack) (void *);
                toggleHeadTrack = (bool (*)(void*)) rdManagerBasePtr->getFunctionByName("toggleHeadTrack");
                (*toggleHeadTrack)((void *) rdManagerBasePtr);
            }
            else if ( c=='\x1b' ) // ESC: 1048603 (0x10001b), LSB: 27 ('\x1b')
            {
                std::cout << "[info] RdInOutHighgui: The escape key was pressed. Bye!" << std::endl;
                rdManagerBasePtr->quit();
            } else if ( c==99 )  // CTRL-C
            {
                std::cout << "[info] RdInOutHighgui: CTRL-C key was pressed. Bye!" << std::endl;
                rdManagerBasePtr->quit();
            }
        }
    }
}

void rdlib::RdInOutHighgui::setRdManagerBasePtr(rdlib::RdManagerBase *rdManagerBasePtr)
{
    this->rdManagerBasePtr = rdManagerBasePtr;
}

void rdlib::RdInOutHighgui::setDisplaySemaphores(sem_t *displaySemaphores)
{
    this->displaySemaphores = displaySemaphores;
}

void rdlib::RdInOutHighgui::setCaptureSemaphores(sem_t *captureSemaphores)
{
    this->captureSemaphores = captureSemaphores;
}


