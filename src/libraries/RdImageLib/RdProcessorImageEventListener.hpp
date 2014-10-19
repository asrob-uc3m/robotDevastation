// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__
#define __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__

#include <math.h>  //-- Just for fabs to make it a bit failproof.

#include <stddef.h> //-- Just for NULL
#include <zbar.h>

#include "RdImageManager.hpp"
#include "RdMacros.hpp"
#include "RdMentalMap.hpp"

namespace rd
{

/**
 * @brief RdProcessorImageEventListener used for processing
 *
 * This object allows to access the received image and has a counter of
 * the incoming images received
 *
 */
class RdProcessorImageEventListener : public RdImageEventListener
{
    public:
        RdProcessorImageEventListener();

        virtual bool onImageArrived( RdImageManager * manager );

        int getImagesArrived();
        void resetImagesArrived();

        RdImage getStoredImage();


    private:
        int images_arrived;
        RdImage stored_image;

        int cameraWidth;
        int cameraHeight;
        bool cameraInitialized;
        //
        zbar::ImageScanner scanner;

        RdMentalMap* mentalMap;
};

}  // namespace rd

#endif  // __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__
