// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ExecutionThread.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>

namespace rdclient {

/************************************************************************/
bool ExecutionThread::init(yarp::os::ResourceFinder &rf) {

    string robot = DEFAULT_ROBOT;
    string cabin = DEFAULT_CABIN;

    cout << "--------------------------------------------------------------" << endl;
    if (rf.check("help")) {
        cout << "ExecutionThread Options:" << endl;
        cout << "\t--robot (name of robot to connect to if any; default: \"" << robot.c_str() << "\")" << endl;
        cout << "\t--cabin (\"ecro\", \"plane\", etc., default \"" << cabin << "\")" << endl;
    }

    if(rf.check("robot")) robot = rf.find("robot").asString();
    cout << "ExecutionThread using robot: " << robot.c_str() << endl;

    if(rf.check("cabin")) cabin = rf.find("cabin").asString();
    cout << "ExecutionThread using cabin: " << cabin << endl;

    cout << "--------------------------------------------------------------" << endl;
    if(rf.check("help")) {
        return false;
    }

    if(robot == "NONE") {
        dev=true;
        // We open a webcam or similar
        cap.open(-1);
        if(!cap.isOpened()) {
            cerr << "[error] No local camera available!" << endl;
            ::exit(0);
        }
        cout << "[success] Local camera available!" << endl;
        // Prepare null pointers for device interfaces
        iPos = NULL;
    } else {
        dev=false;
        string local = "/rdClient";
        yarp::os::Property robotConfig;  // A YARP dictionary class.
        robotConfig.put("device","remote_controlboard");    // device type
        robotConfig.put("local",local+robot);   // name of local port to use
        robotConfig.put("remote",robot);  // name of remote port to connect to
        robotDev.open(robotConfig);
        if (!robotDev.isValid()) {
            cerr << "[error] Could not connect to robot" << endl;
            return false;
        }
        if (!robotDev.view(iPos)) {
            cerr << "[error] Could not connect to robot velocity" << endl;
            return false;
        }

        yarp::os::Property rgbConfig;  // A YARP dictionary class.
        rgbConfig.put("device","remote_grabber");  // device type
        rgbConfig.put("remote",robot+"_rgb/img:o");  // name of remote port to connect to
        rgbConfig.put("local",local+robot+"_rgb/img:i");  // name of local port to use
        //rgbConfig.put("stream","mjpeg");  // carrier to use for streaming, using mjpeg
        //rgbConfig.put("stream","udp");  // carrier to use for streaming, using mjpeg
        rgbDev.open(rgbConfig);
        if (!rgbDev.isValid()) {
            cerr << "[error] No remote camera available!" << endl;
            return false;
        }
        if (!rgbDev.view(iRgb)) {
            cerr << "[error] No remote camera image available!" << endl;
            return false;
        }
        cout << "[success] Remote camera available!" << endl;
    }
    gameControl.setPositionControl(iPos);

    string graphicsPath = "../../resources/rdClient/graphics/";
    string graphicsCabin = graphicsPath + "cabin_" + cabin + ".png";
    picture = imread(graphicsCabin.c_str());
    if(picture.empty()) {
        cerr << "[warning] " << graphicsCabin << " not found." << endl;
        string graphicsCabin2 = ".";
        graphicsCabin2 += graphicsCabin;
        picture = imread(graphicsCabin2.c_str());
        if(picture.empty()) {
            cerr << "[warning] " << graphicsCabin2 << " not found." << endl;
            string graphicsCabin3 = "../";
            graphicsCabin3 += graphicsCabin2;
            picture = imread(graphicsCabin3.c_str());
            if(picture.empty()) {
                cerr << "[error] " << graphicsCabin3 << " not found." << endl;
                return false;
            } else cout << "[success] " << graphicsCabin3 << " found." << endl;
        } else cout << "[success] " << graphicsCabin2 << " found." << endl;
    } else cout << "[success] " << graphicsCabin << " found." << endl;

    string xmlPath = "../../resources/rdClient/xml/";
    string xmlFace = xmlPath + DEFAULT_FACE_CASCADE_NAME;
    if( !faceCascade.load( xmlFace.c_str() ) ){
        cerr << "[warning] " << xmlFace << " not found." << endl;
        string xmlFace2 = ".";
        xmlFace2 += xmlFace;
        if ( !faceCascade.load( xmlFace2.c_str() )) {
            cerr << "[warning] " << xmlFace2 << " not found." << endl;
            string xmlFace3 = "../";
            xmlFace3 += xmlFace2;
            if(!faceCascade.load( xmlFace3.c_str() )) {
                cerr << "[error] " << xmlFace3 << " not found." << endl;
                return false;
            } else cout << "[success] " << xmlFace3 << " found." << endl;
        } else cout << "[success] " << xmlFace2 << " found." << endl;
    } else cout << "[success] " << xmlFace << " found." << endl;

    gameControl.setGun();
    gameControl.resetAmmo();
    maskPic = screenDraw.createChromaMask(picture);
    //namedWindow("Robot Devastation", CV_WINDOW_AUTOSIZE);
    //namedWindow("Robot Devastation", CV_WINDOW_FULLSCREEN);
    namedWindow("Robot Devastation", CV_WINDOW_NORMAL);
    setWindowProperty("Robot Devastation", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
    //cvShowImage("Name", your_image);
    
    // uncomment to see mouse coordinates
    // setMouseCallback("Robot Devastation",gameControl.onMouse);

    this->start();
    return true;
}

/************************************************************************/
void ExecutionThread::run() {  // periodical
    if (dev) { 
        cap.read(videoFrame);
        if(videoFrame.empty())
            return;
    } else { 
        if(!iRgb->getImage(imgRgb)) return;
        IplImage *cvImage = cvCreateImage(cvSize(imgRgb.width(), imgRgb.height()), IPL_DEPTH_8U, 3 );
        cvCvtColor((IplImage*)imgRgb.getIplImage(), cvImage, CV_RGB2BGR);
        videoFrame = cvImage;
    }  
    enemyDetector.humansDetector(videoFrame, faceCascade);
    std::vector< cv::Rect > enemies = enemyDetector.getEnemies();
    gameControl.updateEnemies( &enemies );
    picture.copyTo(videoFrame, maskPic);
    key = waitKey(1);
    gameControl.keyboardActions(videoFrame, key);
    gameControl.showAmmo(videoFrame);
    gameControl.showKills(videoFrame);
    imshow("Robot Devastation", videoFrame);
}

/************************************************************************/


} //rdclient

