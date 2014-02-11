// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_ROBOT_LASER_TOWER_OF_DEATH_HPP__
#define __RD_ROBOT_LASER_TOWER_OF_DEATH_HPP__

#include <utility>
#include <SerialStream.h>

#include "RdRobotBase.hpp"

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdRobotLib
 *
 * @brief The RdRobotLib library contains the \ref RdRobotBase class and some inherited implementations.
 */
namespace rdlib {

/**
 * @ingroup RdRobotLaserTowerOfDeath
 *
 * @brief
 */

class RdRobotLaserTowerOfDeath : public RdRobotBase {
    public:
        RdRobotLaserTowerOfDeath();
        ~RdRobotLaserTowerOfDeath();

        static bool panIncrement();
        static bool panDecrement();

        static bool tiltIncrement();
        static bool tiltDecrement();

        virtual bool shoot();

        virtual bool reset();

    private:
        static const int panRangeMin = 0;
        static const int panRangeMax = 180;
        static const int panStep = 5;
        static const int panInitial = 90;

        static const int tiltRangeMin = 0;
        static const int tiltRangeMax = 180;
        static const int tiltStep = 5;
        static const int tiltInitial = 90;

        static int panJointValue;
        static int tiltJointValue;

        SerialPort * serialPort;

        bool initSerialPort();
        bool checkConnection();

        bool sendCurrentJointValues();
};


}

#endif  // __RD_ROBOT_LASER_TOWER_OF_DEATH_HPP__
