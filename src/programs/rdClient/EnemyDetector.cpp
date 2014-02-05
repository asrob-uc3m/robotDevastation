// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "EnemyDetector.hpp"

namespace rdclient {

/************************************************************************/

void EnemyDetector::humansDetector(Mat& frame, CascadeClassifier faceCascade){

    Mat frame_gray;
    cvtColor( frame, frame_gray, CV_BGR2GRAY );
    faceCascade.detectMultiScale( frame_gray, enemies, 1.1, 2, 0, Size(70, 70));
    for( int i = 0; i < enemies.size(); i++ ){
        rectangle(frame,enemies[i],ORANGE,2);
    }

}

std::vector<Rect> EnemyDetector::getEnemies()
{
    return enemies;
}

/************************************************************************/

} //rdclient

