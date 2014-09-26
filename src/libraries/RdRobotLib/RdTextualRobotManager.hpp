// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_TEXTUAL_ROBOT_MANAGER_HPP__
#define __RD_TEXTUAL_ROBOT_MANAGER_HPP__

#include "RdRobotManager.hpp"

#include "RdMacros.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

namespace rd{

class RdTextualRobotManager : public RdRobotManager
{
    public:
        //-- Robot movement related functions
        virtual bool moveForward(int time = UNUSED, int velocity = UNUSED);
        virtual bool moveBackwards(int time = UNUSED, int velocity = UNUSED);
        virtual bool turnLeft(int time = UNUSED, int velocity = UNUSED);
        virtual bool turnRight(int time = UNUSED, int velocity = UNUSED);

        //-- Robot camera related functions
        virtual bool tiltUp(int time = UNUSED, int velocity = UNUSED);
        virtual bool tiltDown(int time = UNUSED, int velocity = UNUSED);
        virtual bool panLeft(int time = UNUSED, int velocity = UNUSED);
        virtual bool panRight(int time = UNUSED, int velocity = UNUSED);

        //-- Robot connection related functions
        virtual bool connect();
        virtual bool disconnect();
        virtual bool test();
        virtual bool ping();

        //-- Other
        virtual void onDestroy();
        ~RdTextualRobotManager();

        static bool load();

    private:
        RdTextualRobotManager();
        static RdRobotManager * localInstance;

        static const int DISCONNECTED = 0;
        static const int CONNECTED = 1;
        int connection_status;
};
}

bool (*load)(void) = &(rd::RdTextualRobotManager::load);
bool something(void);


#endif //-- __RD_TEXTUAL_ROBOT_MANAGER_HPP__
