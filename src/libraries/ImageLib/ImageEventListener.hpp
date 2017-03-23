// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_EVENT_LISTENER_HPP__
#define __RD_IMAGE_EVENT_LISTENER_HPP__

#include "ImageManager.hpp"

namespace rd{

class ImageManager;

/**
 * @ingroup ImageLib
 *
 * @brief Interface for objects that can be notified of events related to images
 *
 * ImageEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
 */
class ImageEventListener
{
    public:
         /**
         * @brief This function will be called whenever a image arrives to the ImageManager
         * @param manager A reference to the ImageManager that triggered the event
         */
        virtual bool onImageArrived( ImageManager * manager ) = 0;

};

}

#endif //-- __RD_IMAGE_EVENT_LISTENER_HPP__
