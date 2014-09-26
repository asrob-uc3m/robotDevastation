// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ROBOT_MANAGER_HPP__
#define __RD_ROBOT_MANAGER_HPP__

#include "RdMacros.hpp"
#include <string>
#include <vector>
#include <map>

namespace rd{

class RdRobotManager
{
    public:
        //-- Robot movement related functions    
        virtual bool moveForward(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool moveBackwards(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool turnLeft(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool turnRight(int time = UNUSED, int velocity = UNUSED) = 0;

        //-- Robot camera related functions
        virtual bool tiltUp(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool tiltDown(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool panLeft(int time = UNUSED, int velocity = UNUSED) = 0;
        virtual bool panRight(int time = UNUSED, int velocity = UNUSED) = 0;
        
        //-- Robot connection related functions
        virtual bool connect() = 0;
        virtual bool disconnect() = 0;
        virtual bool test() = 0;
        virtual bool ping() = 0;

        //-- Singleton related functions
        static void registerManager(std::string name, RdRobotManager * manager);
        static std::vector<std::string> listAll();
        static RdRobotManager * getRobotManager(std::string name);
        static bool destroyRobotManager();

        //-- Other
        virtual void onDestroy() = 0;
        ~RdRobotManager();

        //-- Constants
        static const int UNUSED = -1;

        RdRobotManager();

    private:
        static RdRobotManager * robotManager;
        static std::map<std::string, RdRobotManager*> robotManagerRegistry;

};
}



#endif //-- __RD_ROBOT_MANAGER_HPP__
