// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __TEO_SIM_RATE_THREAD_HPP__
#define __TEO_SIM_RATE_THREAD_HPP__

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

#define NULL_JMC_MS 20

#define DEFAULT_EXTERN_OBJ "none"
//#define DEFAULT_EXTERN_OBJ "redCan"  // loads plugin

using namespace std;
using namespace OpenRAVE;

namespace rdsim{

/**
 * @brief The fast periodic thread. Launched by RdSim.
 */
class RdSimRateThread : public yarp::os::RateThread {
 public:

    // Set the Thread Rate in the class constructor
    RdSimRateThread() : yarp::os::RateThread(NULL_JMC_MS) {}  // In ms

    void setPenv(const EnvironmentBasePtr& _penv) { penv = _penv; }
    void setPVPencs(vector< yarp::dev::IEncoders* > * _encs) { encs = _encs; }
    void setPVProbots(vector< RobotBasePtr > * _probots) { probots = _probots; }

    void setPcamerasensorbase(vector< SensorBasePtr > * _pcamerasensorbase) { pcamerasensorbase = _pcamerasensorbase; }
    void setPlasersensorbase(vector< SensorBasePtr > * _plasersensorbase) { plasersensorbase = _plasersensorbase; }
    void setPcamerasensordata(vector< boost::shared_ptr<SensorBase::CameraSensorData> > * _pcamerasensordata) { pcamerasensordata = _pcamerasensordata; }
    void setPlasersensordata(vector< boost::shared_ptr<SensorBase::LaserSensorData> > * _plasersensordata) { plasersensordata = _plasersensordata; }
    void setCameraWidth(vector<int> * _cameraWidth) { cameraWidth = _cameraWidth; }
    void setCameraHeight(vector<int> * _cameraHeight) { cameraHeight = _cameraHeight; }
    void setP_imagen(vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* > * _p_imagen) { p_imagen = _p_imagen; }
    void setP_depth(vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* > * _p_depth) { p_depth = _p_depth; }


// -------- RateThread declarations. Implementation in RateThreadImpl.cpp --------

    /**
     * Initialization method. The thread executes this function
     * when it starts and before "run". This is a good place to 
     * perform initialization tasks that need to be done by the 
     * thread itself (device drivers initialization, memory 
     * allocation etc). If the function returns false the thread 
     * quits and never calls "run". The return value of threadInit()
     * is notified to the class and passed as a parameter 
     * to afterStart(). Note that afterStart() is called by the 
     * same thread that is executing the "start" method.
     */
    bool threadInit();

    /**
     * Loop function. This is the thread itself.
     */
    void run();

// ------------------------------- Private -------------------------------------
  protected:
    //
    double jmcMs;
    double lastTime;
    //
    // Rave-specific //
    EnvironmentBasePtr penv;
    vector<RobotBasePtr> * probots;
    //
    std::vector< SensorBasePtr > * pcamerasensorbase;
    std::vector< SensorBasePtr > * plasersensorbase;
    std::vector< boost::shared_ptr<SensorBase::CameraSensorData> > * pcamerasensordata;
    std::vector< boost::shared_ptr<SensorBase::LaserSensorData> > * plasersensordata;
    std::vector<int> * cameraWidth;
    std::vector<int> * cameraHeight;    
    std::vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* > * p_imagen;
    std::vector< yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* > * p_depth;
    //
    // YARP-specific //
    vector< yarp::dev::IEncoders* > * encs;
};

} //rdsim

#endif  // __RD_SIM_HPP__

