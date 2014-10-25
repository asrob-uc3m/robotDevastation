// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __ROBOT_DEVASTATION_HPP__
#define __ROBOT_DEVASTATION_HPP__

#include <yarp/os/RFModule.h>

#include "RdUtils.hpp"
#include "RateThreadOutput.hpp"
#include "RdAudioManager.hpp"
#include "RdMentalMap.hpp"
#include "RdInputManager.hpp"
#include "RdInputEventListener.hpp"
#include "RdYarpNetworkManager.hpp"
#include "RdRobotManager.hpp"
#include "RdRd1RobotManager.hpp"
#include "RdStateMachine.hpp"
#include "RdImageManager.hpp"
#include "RdYarpImageManager.hpp"
#include "RdProcessorImageEventListener.hpp"

namespace rd
{

/**
 * @ingroup robotDevastation
 * @brief The parent Robot Devastation class of the \ref robotDevastation program.
 */
class RobotDevastation : public yarp::os::RFModule, public RdInputEventListener
{
    public:
        /** Called on initialization. */
        virtual bool configure(yarp::os::ResourceFinder &rf);

        /** Released keyboard key. */
        virtual bool onKeyUp(RdKey k);

        /** Pressed keyboard key. */
        virtual bool onKeyDown(RdKey k);

    private:
        RateThreadOutput rateThreadOutput;
        RdInputManager *  inputManager;
        RdAudioManager * audioManager;
        RdMentalMap * mentalMap;
        RdNetworkManager * networkManager;
        RdRobotManager * robotManager;
        RdStateMachine * stateMachine;
        RdImageManager * imageManager;
        RdProcessorImageEventListener processorImageEventListener;

        bool interruptModule();
        double getPeriod();
        bool updateModule();

        bool initSound();

        char* rdRoot;

};

}  // namespace rd

#endif  // __ROBOT_DEVASTATION_HPP__

