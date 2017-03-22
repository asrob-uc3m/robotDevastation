// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_IMAGE_EVENT_LISTENER_HPP__
#define __RD_IMAGE_EVENT_LISTENER_HPP__

#include "ImageManager.hpp"

namespace rd{

class RdImageManager;

/**
 * @ingroup RdImageLib
 *
 * @brief Interface for objects that can be notified of events related to images
 *
 * RdImageEventListener follows the <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> design pattern.
 */
class RdImageEventListener
{
    public:
         /**
         * @brief This function will be called whenever a image arrives to the RdImageManager
         * @param manager A reference to the RdImageManager that triggered the event
         */
        virtual bool onImageArrived( RdImageManager * manager ) = 0;

};

}

#endif //-- __RD_IMAGE_EVENT_LISTENER_HPP__
