// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "ScreenDraw.hpp"

namespace rdclient {

bool ScreenDraw::checkChromaPixel (const Mat& videoFrame, const int i, const int j){
    if(videoFrame.at<Vec3b>(i,j)[0] == 0 &&
            videoFrame.at<Vec3b>(i,j)[1] == 255 &&
            videoFrame.at<Vec3b>(i,j)[2] ==0){
        return true;
    }
    else{
        return false;}
}

/************************************************************************/
Mat ScreenDraw::createChromaMask(const Mat& picture){
    Mat mask = Mat(picture.size(),CV_8UC1,cv::Scalar(0));
    for(int j=0; j< picture.cols; j++){
        for(int i=0; i< picture.rows; i++){
            if(!checkChromaPixel(picture,i,j)){
                mask.at<uchar>(i,j)=255;
            }
        }
    }

    return mask;
}


/************************************************************************/
Rect ScreenDraw::indicateDirection(const int& direction){
    Rect rec;
    if(direction==119){  // w
        rec = Rect(405,373,18,26);
        return rec;
    }
    else if (direction==120){  // x
        rec = Rect(405,404,18,26);
        return rec;
    }
    else if(direction==97){  // a
        rec = Rect(390,393,23,18);
        return rec;
    }
    else if(direction==100){  // d
        rec = Rect(415,393,23,18);
        return rec;
    }

}

/************************************************************************/
Rect ScreenDraw::indicateTilt(const int& up_or_down){
    Rect rec;
    if(up_or_down==107){
        rec = Rect(450,372,15,15);
        return rec;
    }
    else if (up_or_down==109){
        rec = Rect(450,395,15,15);
        return rec;
    }

}

/************************************************************************/
void ScreenDraw::shootLaserGuns(vector<Point>& references){

    references.push_back(Point(215,280));
    references.push_back(Point(300,110));
    references.push_back(Point(425,280));
    references.push_back(Point(340,110));

}

/************************************************************************/

} //rdclient

