#ifndef __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__
#define __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__

#include "RdImageEventListener.hpp"

namespace rd{

/**
 * @brief Dummy RdImageEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received image and has a counter of
 * the incoming images received
 *
 *  @todo Move this class to RdImageLib, set it to be compiled only with the tests
 */
class RdMockupImageEventListener : public RdImageEventListener
{
    public:
        RdMockupImageEventListener();

        virtual bool onImageArrived( RdImageManager * manager );

        int getImagesArrived();
        void resetImagesArrived();

        RdImage getStoredImage();


    private:
        int images_arrived;
        RdImage stored_image;
};
}

#endif // __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__
