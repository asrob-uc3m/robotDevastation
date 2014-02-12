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

        bool panIncrement();
        bool panDecrement();

        bool tiltIncrement();
        bool tiltDecrement();

        virtual bool shoot();
        virtual bool reset();

        //-- Static wrappers:
        static bool panIncrementWrapper(  void * This);
        static bool panDecrementWrapper(  void * This);
        static bool tiltIncrementWrapper( void * This);
        static bool tiltDecrementWrapper( void * This);

    private:
        static const int panRangeMin = 0;
        static const int panRangeMax = 180;
        static const int panStep = 5;
        static const int panInitial = 90;

        static const int tiltRangeMin = 0;
        static const int tiltRangeMax = 180;
        static const int tiltStep = 5;
        static const int tiltInitial = 90;

        int panJointValue;
        int tiltJointValue;

        SerialPort * serialPort;

        bool initSerialPort();
        bool checkConnection();

        bool sendCurrentJointValues();
};


}

#endif  // __RD_ROBOT_LASER_TOWER_OF_DEATH_HPP__
