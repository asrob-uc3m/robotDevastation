// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdSimRateThread.hpp"

namespace rdsim {

// ------------------- RateThread Related ------------------------------------

bool RdSimRateThread::threadInit() {
    printf("[RdSimRateThread] begin: threadInit()\n");
    jmcMs = this->getRate();

    lastTime = yarp::os::Time::now();
    printf("[RdSimRateThread] end: threadInit()\n");
    return true;
}

// -----------------------------------------------------------------------------

void RdSimRateThread::run() {
    //printf("[RdSimRateThread] run()\n");

    int manipulatorCount = 0;
    for(size_t i=0;i<probots->size();i++) {  // For each robot
        int dof = probots->at(i)->GetDOF();  // Create a vector sized
        std::vector<dReal> dEncRaw(dof);     //   its number of joints.
        vector<RobotBase::ManipulatorPtr> pmanipulators = probots->at(i)->GetManipulators();  // Get its manipulators
        for(size_t j=0;j<pmanipulators.size();j++) {
            std::vector< int > manipulatorIndices = pmanipulators[j]->GetArmIndices();
            //int numAxes;
            //encs->at(manipulatorCount)->getAxes(&numAxes);
            //double vals[numAxes];
            double vals[manipulatorIndices.size()];  // experimental
            encs->at(manipulatorCount)->getEncoders(vals);
            for(size_t k=0;k<manipulatorIndices.size();k++)
                dEncRaw[manipulatorIndices[k]] = (vals[k])*M_PI/180.0;
            manipulatorCount++;
        }
        probots->at(i)->SetJointValues(dEncRaw);  // More compatible with physics??
    }

    penv->StepSimulation(jmcMs/1000.0);  // StepSimulation must be given in seconds

    for(unsigned int camIter = 0; camIter<pcamerasensorbase->size(); camIter++ ) {
        pcamerasensorbase->at(camIter)->GetSensorData(pcamerasensordata->at(camIter));
        //std::vector<uint8_t> currentFrame = pcamerasensordata->vimagedata;
        //printf("Vector size: %d\n",currentFrame.size()); // i.e. 480 * 640 * 3 = 921600;
        yarp::sig::ImageOf<yarp::sig::PixelRgb>& i_imagen = p_imagen->at(camIter)->prepare(); 
        i_imagen.resize(cameraWidth->at(camIter),cameraHeight->at(camIter));  // Tamaño de la pantalla
        yarp::sig::PixelRgb p;
        for (int i_x = 0; i_x < i_imagen.width(); ++i_x) {
            for (int i_y = 0; i_y < i_imagen.height(); ++i_y) {
                p.r = pcamerasensordata->at(camIter)->vimagedata[3*(i_x+(i_y*i_imagen.width()))];
                p.g = pcamerasensordata->at(camIter)->vimagedata[1+3*(i_x+(i_y*i_imagen.width()))];
                p.b = pcamerasensordata->at(camIter)->vimagedata[2+3*(i_x+(i_y*i_imagen.width()))];
                i_imagen.safePixel(i_x,i_y) = p;
            }
        }
        p_imagen->at(camIter)->write();
    }
    
    for(unsigned int laserIter = 0; laserIter<plasersensorbase->size(); laserIter++ ) {
        plasersensorbase->at(laserIter)->GetSensorData(plasersensordata->at(laserIter));
        std::vector< RaveVector< dReal > > sensorRanges = plasersensordata->at(laserIter)->ranges;
        std::vector< RaveVector< dReal > > sensorPositions = plasersensordata->at(laserIter)->positions;
        Transform tinv = plasersensordata->at(laserIter)->__trans.inverse();
        // std::vector< dReal > sensorIntensity = plasersensordata[laserIter]->intensity;
        // printf("[%d] sensorPositions size: %d ",laserIter,sensorPositions.size()); // = 1; xyz of the fixed 3d sensor position.
        // printf("sensorRanges size: %d ",sensorRanges.size()); // 64 * 48 = 3072;
        // printf("sensorIntensity size: %d\n",sensorIntensity.size()); // 64 * 48 = 3072;
        //for(unsigned int i=0;i<sensorRanges.size();i++) {
        //   printf("sensorRanges[%d].x: %f  sensorRanges[%d].y: %f  sensorRanges[%d].z: %f sensorIntensity[%d]: %.2f\n",i,sensorRanges[i].x,i,sensorRanges[i].y,i,sensorRanges[i].z,i,sensorIntensity[i]);  // intensity always 1.0 or 0.0 in 3d sensor
        //}*/
        /*Transform tinv = plasersensordata->__trans.inverse();
        for(size_t i = 0; i < _data->ranges.size(); ++i) {
            float* p = (float*)(&_pointcloud2msg.data.at(i*_pointcloud2msg.point_step));
            if( i < _data->positions.size() ) {
                Vector v = tinv*(_data->ranges[i] + _data->positions[i]);
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            } else if( _data->positions.size() > 0 ) {
                Vector v = tinv*(_data->ranges[i] + _data->positions[0]);
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            } else {
                Vector v = tinv*_data->ranges[i];
                p[0] = (float)v.x;
                p[1] = (float)v.y;
                p[2] = (float)v.z;
            }
            if( _data->intensity.size()==_data->ranges.size() ) {
                p[3] = _data->intensity[i];
            }
        }*/
        yarp::sig::ImageOf<yarp::sig::PixelInt>& i_depth = p_depth->at(laserIter)->prepare();
        if(sensorRanges.size()==3072) i_depth.resize(64,48);  // Tamaño de la pantalla (64,48)
        else if(sensorRanges.size()==12288) i_depth.resize(128,96);
        else if(sensorRanges.size()==49152) i_depth.resize(256,192);
        else if(sensorRanges.size()==307200) i_depth.resize(640,480);
        else if(sensorRanges.size()==4) i_depth.resize(2,2);
        //else printf("[warning] unrecognized laser sensor data size.\n");
        else i_depth.resize(sensorRanges.size(),1);
        for (int i_y = 0; i_y < i_depth.height(); ++i_y) {  // was y in x before
            for (int i_x = 0; i_x < i_depth.width(); ++i_x) {
                //double p = sensorRanges[i_y+(i_x*i_depth.height())].z;
                double p;
                if( sensorPositions.size() > 0 ) {
                    Vector v = tinv*(sensorRanges[i_y+(i_x*i_depth.height())] + sensorPositions[0]);
                    p = (float)v.z;
                } else {
                    Vector v = tinv*(sensorRanges[i_y+(i_x*i_depth.height())]);
                    p = (float)v.z;
                }
                i_depth(i_x,i_y) = p*1000.0;  // give mm
            }
        }
        p_depth->at(laserIter)->write();
    }

}

// -----------------------------------------------------------------------------

} //rdsim

