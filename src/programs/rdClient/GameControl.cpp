// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "GameControl.hpp"

namespace rdclient {

/************************************************************************/
void GameControl::setVelocityControl(yarp::dev::IVelocityControl* _iVel) {
    iVel = _iVel;
}

/************************************************************************/
void GameControl::onMouse( int event, int x, int y, int flags, void* param ) {
    if ( event==CV_EVENT_MOUSEMOVE ) {
       cout << "x: " << x << " y: " << y << endl;
    }
}

/************************************************************************/
void GameControl::keyboardActions(Mat& image, const int key){

    // 199 w, 115 s, 97 a, 100 d
    // 107 k (up), 109 m (down)
    // 32 space (fire)

    // 27 escape (The end)
    //114 R reload

    // 49 50 51 - 1 2 3 gun change

    if (key==27) {
        exit(0);
    }
    
    else if (key==115) { // s
        double vels[2] = {1500,1500};
        if(iVel) iVel->velocityMove(vels);
    }
    
    else if (key==119 || key==120 || key==97 || key==100) {
        ScreenDraw sd;
        rectangle(image,sd.indicateDirection(key), YELLOW, 2);
        if (key==119) { // w
            double vels[2] = {1600,1600};
            if(iVel) iVel->velocityMove(vels);
        } else if (key==120) {  // x
            double vels[2] = {1400,1400};
            if(iVel) iVel->velocityMove(vels);
        } else if (key==97) { // a
            double vels[2] = {1600,1400};
            if(iVel) iVel->velocityMove(vels);
        } else if (key==100) { // d
            double vels[2] = {1400,1600};
            if(iVel) iVel->velocityMove(vels);
        }
    }
    
    else if (key==107 || key==109) { // k m
        ScreenDraw sd2;
        rectangle(image, sd2.indicateTilt(key), GREEN, 2);
    }
    
    else if(key==49 || key==50 || key==51) { // 1 2 3
         setGun(key);
    }
    
    else if(key==114){
         resetAmmo();
    }
    
    else if (key==32 && ammo>0) {
        vector<Point> references;
        ScreenDraw sd3;
        sd3.shootLaserGuns(references);
        line(image,references[0],references[1],gunColor, gunThickness);
        line(image,references[2],references[3],gunColor, gunThickness);
        reduceAmmo();
    }
}

/************************************************************************/

void GameControl::setGun(int key){
    if (key==49){
        gunColor = RED;
        gunThickness= 3;
    }

    else if (key==50){
        gunColor = BLUE;
        gunThickness= 12;
    }

    else if (key==51){
        gunColor = GREEN;
        gunThickness= 7;
    }
}
/************************************************************************/

void GameControl::showAmmo(Mat &image){
    stringstream ss;
    ss << ammo;
    string count = "Ammo " + ss.str();
    Point point1 = Point(image.cols*0.8, image.rows*0.05);
    putText(image,count,point1,CV_FONT_HERSHEY_SIMPLEX,0.8,YELLOW,2);
}

/************************************************************************/

} //rdclient

