// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RateThreadOutput.hpp"

void rd::RateThreadOutput::init(yarp::os::ResourceFinder &rf)
{
    int rateMs = DEFAULT_RATE_MS;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RateThreadOutput options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
    }
    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();

    printf("RateThreadOutput using rateMs: %d.\n", rateMs);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    this->setRate(rateMs);
    this->start();

}

void rd::RateThreadOutput::run()
{
    //printf("[SegmentorThread] run()\n");

    yarp::sig::ImageOf<yarp::sig::PixelRgb> *inYarpImg = pInImg->read(false);
    if (inYarpImg==NULL) {
        printf("No img yet...\n");
        return;
    };
    
    // {yarp ImageOf Rgb -> openCv Mat Bgr}
    //IplImage *inIplImage = cvCreateImage(cvSize(inYarpImg->width(), inYarpImg->height()),
    //                                     IPL_DEPTH_8U, 3 );
    //cvCvtColor((IplImage*)inYarpImg->getIplImage(), inIplImage, CV_RGB2BGR);
    //Mat inCvMat(inIplImage);


}

void rd::RateThreadOutput::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}

