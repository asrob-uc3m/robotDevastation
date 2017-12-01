// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__
#define __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__

#include <string>

#include <zbar.h>

#include "ImageManager.hpp"
#include "ImageEventListener.hpp"
#include "MentalMap.hpp"

namespace rd
{

/**
 * @ingroup ImageLib
 *
 * @brief ProcessorImageEventListener used for processing
 *
 * This object allows to access the received image and has a counter of
 * the incoming images received.
 *
 */
class ProcessorImageEventListener : public ImageEventListener
{
    public:
        ProcessorImageEventListener();
        virtual ~ProcessorImageEventListener();

        virtual bool onImageArrived( ImageManager * manager );


    private:
        bool isInteger(const std::string & s) const;
        Image received_image;

        int cameraWidth;
        int cameraHeight;
        bool cameraInitialized;
        unsigned char* rimage;

        zbar::ImageScanner scanner;

        MentalMap* mentalMap;
};

}  // namespace rd

#endif  // __RD_PROCESSOR_IMAGE_EVENT_LISTENER_HPP__
