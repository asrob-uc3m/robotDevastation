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
        cameraWidth = inYarpImg->width();
        cameraHeight = inYarpImg->height();
        cameraInitialized = true;
    }

    //-- Convert from YARP rgb to zbar b/w.
    unsigned char* rimage = (unsigned char*)malloc( cameraWidth * cameraHeight );

    for( unsigned y = 0; y < cameraHeight; y++ )
    {
        for( unsigned x = 0; x < cameraWidth; x++ )
        {
            //Convert to Y800
            int data = inYarpImg->pixel(x,y).r + inYarpImg->pixel(x,y).g + inYarpImg->pixel(x,y).b;
            data/=3;
            if( data < 0 )
                data = 0;
            if( data > 255 )
                data = 255;
            rimage[x+y*cameraWidth] = data/3;
        }
    }
    zbar::Image image(cameraWidth, cameraHeight, "Y800", rimage, cameraWidth * cameraHeight);

    int n = scanner.scan(image);
    //printf("How many detected QR: %d\n",n);


    std::vector< RdTarget > targets;

    // extract results
    for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
    {
        std::vector<cv::Point> vp;
        // do something useful with results
        RD_INFO("[%s]: %s\n", symbol->get_type_name().c_str(), symbol->get_data().c_str());
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++){
            vp.push_back(cv::Point(symbol->get_location_x(i),symbol->get_location_y(i)));
        }
        cv::RotatedRect r = minAreaRect(vp);
        cv::Point2f pts[4];
        r.points(pts);

        std::stringstream identifier_str(symbol->get_data());
        int identifier_int;
        identifier_str >> identifier_int;
        RD_INFO("QR id: %d.\n",identifier_int);


        RdTarget target( identifier_int,
                       RdVector2d(r.center.x,r.center.y),
                       RdVector2d(r.size.width, r.size.height) );
        targets.push_back(target);

    }

    mentalMap->updateTargets(targets);

}

void rd::RateThreadProcess::setInImg(yarp::os::BufferedPort<yarp::sig::ImageOf<yarp::sig::PixelRgb> > * pInImg)
{
    this->pInImg = pInImg;
}
