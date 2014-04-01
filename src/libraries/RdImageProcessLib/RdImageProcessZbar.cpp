// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdImageProcessZbar.hpp"

bool rdlib::RdImageProcessZbar::setup()
{
    scanner.set_config( zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
    return true;
}

bool rdlib::RdImageProcessZbar::process(char * imgPtr, const int width, const int height, const int step,
                                          std::vector<std::pair<int, int> > &enemyPos,
                                          std::vector<double> &enemySize)
{
    //-- Put image in cv::Mat
    //cv::Mat cvimage(cv::Size(width, height), CV_8UC3, imgPtr, step);
    cv::Mat cvimage(cv::Size(width, height), CV_8UC3, imgPtr, step);
    cv::Mat cvimagetreat;
    cvtColor(cvimage,cvimagetreat,CV_RGB2GRAY);

    uchar *raw = (uchar *)cvimagetreat.data;
    // wrap image data
    zbar::Image image(width, height, "Y800", raw, width * height);
    // scan the image for barcodes
    int n = scanner.scan(image);
    // extract results

    enemiesFound.clear();
    enemiesFoundSize.clear();

    for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
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
        enemiesFound.push_back( std::pair< int, int>( r.center.x, r.center.y ));
        enemiesFoundSize.push_back( pts[1].x - pts[0].x );
    }
    cvimage.release();
    cvimagetreat.release();

    enemyPos = enemiesFound;
    enemySize = enemiesFoundSize;

    return true;
}

