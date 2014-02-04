// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EnemyDetector.hpp"

namespace rdclient {

/************************************************************************/

void EnemyDetector::humansDetector(Mat& frame, CascadeClassifier faceCascade){

    std::vector<Rect> faces;
    Mat frame_gray;
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    faceCascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0, Size(70, 70));
    for( int i = 0; i < faces.size(); i++ ){
        rectangle(frame,faces[i],ORANGE,2);
    }
}

/************************************************************************/

} //rdclient

