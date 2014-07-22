// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RateThreadProcess.hpp"

void rd::RateThreadProcess::init(yarp::os::ResourceFinder &rf)
{
    rateMs = DEFAULT_RATE_MS;
    cameraInitialized = false;

    printf("--------------------------------------------------------------\n");
    if (rf.check("help")) {
        printf("RateThreadProcess options:\n");
        printf("\t--help (this help)\t--from [file.ini]\t--context [path]\n");
        printf("\t--rateMs (default: \"%d\")\n",rateMs);
    }
    if (rf.check("rateMs")) rateMs = rf.find("rateMs").asInt();

    printf("RateThreadProcess using rateMs: %d.\n", rateMs);

    printf("--------------------------------------------------------------\n");
    if(rf.check("help")) {
        ::exit(1);
    }

    scanner.set_config( zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);

    //cv::namedWindow("OpenCV", CV_WINDOW_NORMAL);  //opencv viewer//

    this->setRate(rateMs);
    this->start();

}

void rd::RateThreadProcess::run()
{
    //printf("[RateThreadProcess] run()\n");

    yarp::sig::ImageOf<yarp::sig::PixelRgb> *inYarpImg = pInImg->read(false);
    if (inYarpImg==NULL) {
        //printf("No img yet...\n");
        return;
    };
    
    if(!cameraInitialized)
    {
        cameraDepth=8;  // the depth of the surface in bits
        cameraChannels=3;  // R G B
        cameraWidthstep = inYarpImg->getRowSize(); // the length of a row of pixels in bytes 1920 for 640x480
        cameraWidth = inYarpImg->width();
        cameraHeight = inYarpImg->height();
        cameraInitialized = true;
    }

    //void *raw = (void *)inYarpImg->getRawImage();

    //-- Use opencv for now
    IplImage *iplImage = cvCreateImage(cvSize(cameraWidth, cameraHeight), IPL_DEPTH_8U, 3 );
    cvCvtColor((IplImage*)inYarpImg->getIplImage(), iplImage, CV_RGB2BGR);
    cv::Mat cvimage = iplImage;
    cv::Mat cvimagetreat;
    cvtColor(cvimage,cvimagetreat,CV_BGR2GRAY);
    //cv::imshow("OpenCV", cvimagetreat);  //opencv viewer//
    //cv::waitKey( 1 );  //opencv viewer//

    uchar *raw = (uchar *)cvimagetreat.data;

    // wrap image data
    zbar::Image image(cameraWidth, cameraHeight, "Y800", raw, cameraWidth * cameraHeight);

    //zbar::Image image(cameraWidth, cameraHeight, "GREY", raw, cameraWidth * cameraHeight);  //remove//
    //image.convert( *(int*)"Y800",cameraWidth, cameraHeight);  //remove//
    int n = scanner.scan(image);
    printf("%d\n",n);
    // extract results    


    for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
        //std::vector<cv::Point> vp;
        // do something useful with results
        RD_INFO("[%s]: %s\n", symbol->get_type_name().c_str(), symbol->get_data().c_str());
        /*int n = symbol->get_location_size();
        for(int i=0;i<n;i++){
            vp.push_back(cv::Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }
        cv::RotatedRect r = minAreaRect(vp);
        cv::Point2f pts[4];
        r.points(pts);
        //enemiesFound.push_back( std::pair< int, int>( r.center.x, r.center.y ));
        //enemiesFoundSize.push_back( pts[1].x - pts[0].x );
        //RdEnemy enemy(localVectorOfRdEnemy.size(), RdEnemy::QR_CODE, RdVector2d( r.center.x, r.center.y ), pts[1].x-pts[0].x, pts[2].y-pts[0].y );
        std::stringstream identifier_str(symbol->get_data());
        int identifier_int;
        identifier_str >> identifier_int;
        RD_INFO("QR RdEnemy id: %d.\n",identifier_int);
        RdEnemy enemy(identifier_int, RdEnemy::QR_CODE, RdVector2d( r.center.x, r.center.y ), pts[1].x-pts[0].x, pts[2].y-pts[0].y );
        localVectorOfRdEnemy.push_back(enemy);*/
    }
}

void rd::RateThreadProcess::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg) {
    this->pInImg = pInImg;
}

