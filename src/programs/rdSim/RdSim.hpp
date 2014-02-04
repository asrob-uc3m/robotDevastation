// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_SIM_HPP__
#define __RD_SIM_HPP__

#include <yarp/os/all.h>
#include <yarp/os/Semaphore.h>
#include <yarp/dev/ControlBoardInterfaces.h>
#include <yarp/dev/Drivers.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/sig/all.h>

#include <openrave-core.h>

#include <stdio.h>
#include <iostream>
#include <sstream>

#include "RdSimRateThread.hpp"

#define DEFAULT_ENV "rdSim_default.env.xml"
#define DEFAULT_JMC_MS 20.0  // [ms]
#define DEFAULT_PHYSICS "none"
#define DEFAULT_VIEWER 1

using namespace std;
using namespace OpenRAVE;

namespace rdsim{

/**
 * @ingroup rdSim
 *
 * @brief The main \ref rdSim program class, implemented as a slow watchdog class that also launches an \ref RdSimRateThread.
 */
class RdSim : public yarp::os::RFModule {
  public:

// -------- RFModule declarations. Implementation in RdSim.cpp --------

    /**
     * Configure the module, pass a ResourceFinder object to the module.
     *
     * @param rf a previously initialized ResourceFinder
     * @return true/false upon success/failure
     *
     * \note attachTerminal() is no longer called automatically. You
     * can call it in the configure function.
     */
    virtual bool configure(yarp::os::ResourceFinder &rf);

    /**
     * You can override this to control the approximate periodicity at which
     * updateModule() is called by runModule().  By default, it returns
     * 1.0. Time here is in seconds.
     *
     * @return the desired period between successive calls to updateModule()
     */
    virtual double getPeriod();

    /**
     * Override this to do whatever your module needs to do.
     *
     * When your module wants to stop, return false.  The module's actual
     * work could be done during this call, or it could just check the
     * state of a thread running in the background.
     *
     * @return true if module should continue
    */
    virtual bool updateModule();

    /**
     * Close function.
     *
     * This is called automatically when the module closes, after the last call
     * to updateModule.
     * Override this to perform memory cleanup or other activities.
     *
     * @return true/false on success failure.
     */
    virtual bool close();

// ------------------------------- Protected -------------------------------------
  protected:
    /**
     * An instance of the fast periodical thread.
     */
    RdSimRateThread rdSimRateThread;

    // Pointers to stuff...
    EnvironmentBasePtr penv;
    PhysicsEngineBasePtr pe;
    vector<RobotBasePtr> probots;
    //
    boost::thread_group orThreads;
    //
    std::vector< SensorBasePtr > pcamerasensorbase;
    std::vector< SensorBasePtr > plasersensorbase;
    std::vector< boost::shared_ptr<SensorBase::CameraSensorData> > pcamerasensordata;
    std::vector< boost::shared_ptr<SensorBase::LaserSensorData> >  plasersensordata;
    std::vector<int> cameraWidth;
    std::vector<int> cameraHeight;    
    std::vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* > p_imagen;
    std::vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* > p_depth;
    //
    bool createManipulatorDevice(const string& manipulatorPortName, const int& axes);
    vector<yarp::dev::PolyDriver*> manipulatorDevices;
    vector<yarp::dev::IEncoders*> vpencs;
};

} //rdsim

#endif  // __RD_SIM_HPP__

