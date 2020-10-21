// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "ProcessorImageEventListener.hpp"

#include <cstdlib> //-- malloc(), free(), abs(), strtol()
#include <cctype>  //-- isdigit()
#include <cstddef> //-- NULL
#include <vector>
#include <sstream>

#include "Target.hpp"
#include "Vector2dBase.hpp"

#include <ColorDebug.h>

rd::ProcessorImageEventListener::ProcessorImageEventListener()
{
    //images_arrived = 0;
    cameraInitialized = false;
    cameraWidth = cameraHeight = 0;
    rimage = NULL;

    mentalMap = MentalMap::getMentalMap();

    scanner.set_config( zbar::ZBAR_NONE, zbar::ZBAR_CFG_ENABLE, 1);
}

rd::ProcessorImageEventListener::~ProcessorImageEventListener()
{
    if (rimage!=NULL)
    {
        std::free(rimage);
        rimage = NULL;
    }
}

bool rd::ProcessorImageEventListener::onImageArrived( ImageManager * manager )
{
    received_image = manager->getImage();
    //images_arrived++;
    //CD_DEBUG("%d\n",images_arrived);

    if(!cameraInitialized)
    {
        cameraWidth = received_image.width();
        cameraHeight = received_image.height();
        cameraInitialized = true;
        rimage = (unsigned char*)std::malloc( cameraWidth * cameraHeight );
    }

    //-- Convert from YARP rgb to zbar b/w.

    for( int y = 0; y < cameraHeight; y++ )
    {
        for( int x = 0; x < cameraWidth; x++ )
        {
            //Convert to Y800
            int data = received_image.pixel(x,y).r + received_image.pixel(x,y).g + received_image.pixel(x,y).b;
            data/=3;
            if( data < 0 )
                data = 0;
            if( data > 255 )
                data = 255;
            rimage[x+y*cameraWidth] = data/3;
        }
    }
    zbar::Image image(cameraWidth, cameraHeight, "Y800", rimage, cameraWidth * cameraHeight);

    int numDetected = scanner.scan(image);
    //CD_DEBUG("How many detected QR: %d\n",numDetected);

    std::vector< Target > targets;

    // extract results
    for(zbar::Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol)
    {
        //-- Obtain id from QR.
        int identifier_int;
        std::stringstream identifier_str(symbol->get_data());
        if (isInteger(identifier_str.str()))
        {
            identifier_str >> identifier_int;
            CD_INFO("QR id: %d.\n",identifier_int);
        }
        else
        {
            CD_WARNING("Ignoring QR with id:\"%s\".\n", identifier_str.str().c_str());
            continue;
        }

        //-- Obtain coordinates from QR.
        std::vector< Vector2d > coords;
        int n = symbol->get_location_size();
        for(int i=0;i<n;i++)
        {
             Vector2d coord(symbol->get_location_x(i),symbol->get_location_y(i));
             //-- Check the following output if things ever start to fail.
             //CD_DEBUG("%d: %d %d\n",i,coord.x,coord.y);
             coords.push_back(coord);
        }
        int qrWidth = std::abs(coords[2].getX() - coords[1].getX());
        int qrHeight = std::abs(coords[1].getY() - coords[0].getY());
        Vector2d qrCenter(coords[0].getX() + (qrWidth / 2), coords[0].getY() + (qrHeight / 2) );

        Target target( identifier_int,
                         qrCenter,
                         Vector2d(qrWidth, qrHeight) );
        targets.push_back(target);

    }

    mentalMap->updateTargets(targets);
    return true;
}

bool rd::ProcessorImageEventListener::isInteger(const std::string & s) const
{
    //-- Code adapted from: http://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int
   if(s.empty() || ((!std::isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   std::strtol(s.c_str(), &p, 10);

   return (*p == 0);
}
