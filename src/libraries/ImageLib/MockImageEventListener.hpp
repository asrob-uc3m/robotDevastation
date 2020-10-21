// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_MOCK_IMAGE_EVENT_LISTENER_HPP__
#define __RD_MOCK_IMAGE_EVENT_LISTENER_HPP__

#include "ImageManager.hpp"
#include "ImageEventListener.hpp"

namespace rd{

/**
 * @ingroup ImageLib
 *
 * @brief Dummy ImageEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mock object allows to access the received image and has a counter of
 * the incoming images received.
 */
class MockImageEventListener : public ImageEventListener
{
    public:
        MockImageEventListener();

        virtual bool onImageArrived( ImageManager * manager );

        int getImagesArrived() const;
        void resetImagesArrived();

        Image getStoredImage();


    private:
        int images_arrived;
        Image stored_image;
};
}

#endif // __RD_MOCK_IMAGE_EVENT_LISTENER_HPP__
