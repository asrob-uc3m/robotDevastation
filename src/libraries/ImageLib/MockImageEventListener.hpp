#ifndef __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__
#define __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__

#include "ImageEventListener.hpp"

namespace rd{

/**
 * @brief Dummy ImageEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received image and has a counter of
 * the incoming images received
 *
 *  @todo Move this class to ImageLib, set it to be compiled only with the tests
 */
class MockupImageEventListener : public ImageEventListener
{
    public:
        MockupImageEventListener();

        virtual bool onImageArrived( ImageManager * manager );

        int getImagesArrived();
        void resetImagesArrived();

        Image getStoredImage();


    private:
        int images_arrived;
        Image stored_image;
};
}

#endif // __RD_MOCKUP_IMAGE_EVENT_LISTENER_HPP__
