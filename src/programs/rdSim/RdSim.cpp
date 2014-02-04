// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdSim.hpp"

namespace rdsim {

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername, int _viewer);

// ------------------- RFModule Related ------------------------------------

bool RdSim::configure(yarp::os::ResourceFinder &rf) {

    string env = DEFAULT_ENV;
    double jmcMs = DEFAULT_JMC_MS;
    string physics = DEFAULT_PHYSICS;
    int viewer = DEFAULT_VIEWER;

    if(rf.check("help")) {
        printf("RdSim options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");

        printf("\t--env [xml] (env in abs, default: \"%s\")\n",env.c_str());
        printf("\t--physics [type] (type of physics, default: \"%s\")\n",physics.c_str());
        printf("\t--viewer [type] (set to 0 for none, default: \"%d\")\n",viewer);
    }

    if (rf.check("env")) env = rf.find("env").asString();
    if (rf.check("jmcMs")) jmcMs = rf.find("jmcMs").asDouble();
    if (rf.check("physics")) physics = rf.find("physics").asString();
    if (rf.check("viewer")) viewer = rf.find("viewer").asInt();

    printf("RdSim using context: %s.\n",rf.getContextPath().c_str());
    printf("RdSim using env: %s, jmcMs: %f, physics: %s, viewer: %d\n.", env.c_str(), jmcMs, physics.c_str(), viewer);

    if(rf.check("help")) {
        ::exit(1);
    }

    // Initialize OpenRAVE-core
    RaveInitialize(true);  // Start openrave core
    penv = RaveCreateEnvironment();  // Create the main OpenRAVE environment, set the EnvironmentBasePtr
    penv->SetDebugLevel(Level_Debug);  // Relatively new function
    penv->StopSimulation();  // NEEDED??
    boost::thread thviewer(boost::bind(SetViewer,penv,"qtcoin",viewer));
    orThreads.add_thread(&thviewer);
    yarp::os::Time::delay(0.4); // wait for the viewer to init, in [s]

    string envFull(rf.getContextPath());
    envFull += "/../models/";
    envFull += env;
    if (!penv->Load(envFull.c_str())) {
        fprintf(stderr,"[RdSim] error: Could not load %s environment.\n",envFull.c_str());
        if (!penv->Load(env.c_str())) {
            fprintf(stderr,"[RdSim] error: Could not load %s environment.\n",env.c_str());
            return false;
        }
    }
    printf("[RdSim] success: Loaded environment.\n");

    // Attach a physics engine
    if(physics=="ode"){
        penv->SetPhysicsEngine(RaveCreatePhysicsEngine(penv,"ode"));
        penv->GetPhysicsEngine()->SetGravity(OpenRAVE::Vector(0,0,-9.8));
    }

    //-- Get robots
    penv->GetRobots(probots);
    for(size_t i=0;i<probots.size();i++) {
        //printf( "robots[%ld]: %s\n",i,probots[i]->GetName().c_str());
        vector<RobotBase::ManipulatorPtr> pmanipulators = probots[i]->GetManipulators();
        for(size_t j=0;j<pmanipulators.size();j++) {
            //printf( "* manipulators[%ld]: %s\n",j,pmanipulators[j]->GetName().c_str() );
            string manipulatorPortName("/");
            manipulatorPortName += probots[i]->GetName();
            manipulatorPortName += "/";
            manipulatorPortName += pmanipulators[j]->GetName();
            printf( "manipulatorPortName: %s\n",manipulatorPortName.c_str() );
            std::vector< int > manipulatorIndices = pmanipulators[j]->GetArmIndices();
            printf( "manipulatorIndices:");
            for(size_t k=0;k<manipulatorIndices.size();k++)
                printf(" %d",manipulatorIndices[k]);
            printf(".\n");
            createManipulatorDevice(manipulatorPortName,manipulatorIndices.size());
        }
    }

    for ( unsigned int robotIter = 0; robotIter<probots.size(); robotIter++ ) {
        std::vector<RobotBase::AttachedSensorPtr> sensors = probots.at(robotIter)->GetAttachedSensors();
        printf("Sensors found on robot %d (%s): %d.\n",robotIter,probots.at(robotIter)->GetName().c_str(),(int)sensors.size());
        for ( unsigned int sensorIter = 0; sensorIter<sensors.size(); sensorIter++ ) {
            SensorBasePtr psensorbase = sensors.at(sensorIter)->GetSensor();
            std::string tipo = psensorbase->GetName();
            printf("Sensor %d name: %s\n",sensorIter,tipo.c_str());
            // printf("Sensor %d description: %s\n",sensorIter,psensorbase->GetDescription().c_str());
            if (psensorbase->Supports(SensorBase::ST_Camera)) {
                printf("Sensor %d supports ST_Camera.\n", sensorIter);
                // Activate the camera
                psensorbase->Configure(SensorBase::CC_PowerOn);
                // Show the camera image in a separate window
                // pcamerasensorbase->Configure(SensorBase::CC_RenderDataOn);
                // Get some camera parameter info
                boost::shared_ptr<SensorBase::CameraGeomData> pcamerageomdata = boost::dynamic_pointer_cast<SensorBase::CameraGeomData>(psensorbase->GetSensorGeometry(SensorBase::ST_Camera));
                // printf("Camera width: %d, height: %d.\n",pcamerageomdata->width,pcamerageomdata->height);
                cameraWidth.push_back(pcamerageomdata->width);
                cameraHeight.push_back(pcamerageomdata->height);
                // Get a pointer to access the camera data stream
                pcamerasensordata.push_back(boost::dynamic_pointer_cast<SensorBase::CameraSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Camera)));
                pcamerasensorbase.push_back(psensorbase);  // "save"
                yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >* tmpPort = new yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> >;
                string tmpName("/");
                string cameraSensorString(psensorbase->GetName());
                size_t pos = cameraSensorString.find("imageMap");
                if ( pos != std::string::npos) {
                    tmpName += cameraSensorString.substr (0,pos-1);
                    tmpName += "/imageMap:o";
                } else {
                    tmpName += cameraSensorString.c_str();
                    tmpName += "/img:o";
                }
                tmpPort->open(tmpName);
                p_imagen.push_back(tmpPort);
            } else if (psensorbase->Supports(SensorBase::ST_Laser)) {
                printf("Sensor %d supports ST_Laser.\n", sensorIter);
                // Activate the sensor
                psensorbase->Configure(SensorBase::CC_PowerOn);
                // Paint the rays in the OpenRAVE viewer
                psensorbase->Configure(SensorBase::CC_RenderDataOn);
                // Get some laser parameter info
                // boost::shared_ptr<SensorBase::LaserGeomData> plasergeomdata = boost::dynamic_pointer_cast<SensorBase::LaserGeomData>(psensorbase->GetSensorGeometry(SensorBase::ST_Laser));
                // printf("Laser resolution: %f   %f.\n",plasergeomdata->resolution[0],plasergeomdata->resolution[1]);
                // printf("Laser min_angle: %f   %f.\n",plasergeomdata->min_angle[0],plasergeomdata->min_angle[1]);
                // printf("Laser max_angle: %f   %f.\n",plasergeomdata->max_angle[0],plasergeomdata->max_angle[1]);
                // Get a pointer to access the laser data stream
                plasersensordata.push_back(boost::dynamic_pointer_cast<SensorBase::LaserSensorData>(psensorbase->CreateSensorData(SensorBase::ST_Laser)));
                plasersensorbase.push_back(psensorbase);  // "save"
                yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >* tmpPort = new yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelInt> >;
                string tmpName("/");
                string depthSensorString(psensorbase->GetName());
                size_t pos = depthSensorString.find("depthMap");
                if ( pos != std::string::npos) {
                    tmpName += depthSensorString.substr (0,pos-1);
                    tmpName += "/depthMap:o";
                } else {
                    tmpName += depthSensorString.c_str();
                    tmpName += "/depth:o";
                }
                tmpPort->open(tmpName);
                p_depth.push_back(tmpPort);
            } else printf("Sensor %d not supported.\n", robotIter);
        }
    }

    // Start the RateThread
    rdSimRateThread.setRate(jmcMs);
    rdSimRateThread.setPenv(penv);
    rdSimRateThread.setPVProbots(&probots);
    rdSimRateThread.setPVPencs(&vpencs);

    rdSimRateThread.setPcamerasensorbase(&pcamerasensorbase);
    rdSimRateThread.setPcamerasensordata(&pcamerasensordata);
    rdSimRateThread.setP_imagen(&p_imagen);
    rdSimRateThread.setCameraWidth(&cameraWidth);
    rdSimRateThread.setCameraHeight(&cameraHeight);
    rdSimRateThread.setPlasersensorbase(&plasersensorbase);
    rdSimRateThread.setPlasersensordata(&plasersensordata);
    rdSimRateThread.setP_depth(&p_depth);

    rdSimRateThread.start();
    
    return true;
}

// -----------------------------------------------------------------------------

bool RdSim::createManipulatorDevice(const string& manipulatorPortName, const int& axes) {
    printf("[RdSim] begin: createManipulatorDevice()\n");
    yarp::os::Property options;
    options.put("device","controlboard");
    //j//options.put("subdevice","ravepart");
    options.put("subdevice","test_motor");
    options.put("axes",axes);
    options.put("name",manipulatorPortName);
    yarp::dev::PolyDriver* dd = new yarp::dev::PolyDriver(options);
    manipulatorDevices.push_back(dd);
    yarp::dev::IEncoders *encs;
    dd->view(encs);
    vpencs.push_back(encs);
    printf("[RdSim] end: createManipulatorDevice()\n");
    return true;
}

// -----------------------------------------------------------------------------

double RdSim::getPeriod() {
    return 5.0;
}

// -----------------------------------------------------------------------------

bool RdSim::updateModule() {
    printf("RdSim alive...\n");
    return true;
}

// -----------------------------------------------------------------------------

bool RdSim::close() {
    printf("[RdSim] begin: close(). Ask thread to stop...\n");
    rdSimRateThread.askToStop();
    printf("[RdSim] Done. Closing devices...\n");
    for (size_t i=0;i<manipulatorDevices.size();i++) {
        manipulatorDevices[i]->close();
        delete manipulatorDevices[i];
        manipulatorDevices[i] = 0;
    }
    //penv->StopSimulation();  // NEEDED??
    printf("[RdSim] Devices closed. Closing environment...\n");
    penv->Destroy(); // destroy
    yarp::os::Time::delay(0.4);
    printf("[RdSim] close() joining...\n");
    orThreads.join_all(); // wait for the viewer thread to exit
    printf("[RdSim] success: join_all ended.\n");
    printf("[RdSim] end: close().\n");
    return true;
}

// -----------------------------------------------------------------------------

void SetViewer(EnvironmentBasePtr penv, const std::string& viewername, int _viewer)
{
    ViewerBasePtr viewer = RaveCreateViewer(penv,viewername);
    BOOST_ASSERT(!!viewer);

    // attach it to the environment:
    penv->AddViewer(viewer);  // penv->AttachViewer(viewer);

    // finally you call the viewer's infinite loop (this is why you need a separate thread):
    bool showgui = true; // change to false to disable scene view
    if(!_viewer) showgui = false;  // if viewer arg = 0
    viewer->main(showgui);
}

// -----------------------------------------------------------------------------

} //rdsim

