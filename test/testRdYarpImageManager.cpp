#include "gtest/gtest.h"

#include "RdImageManager.hpp"
#include "RdYarpImageManager.hpp"
#include "RdImageEventListener.hpp"

using namespace rd;

/**
 * @brief Dummy RdImageEventListener used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This mockup object allows to access the received image and has a counter of
 * the incoming images received
 *
 */
class RdMockupImageEventListener : public RdImageEventListener
{
    public:
        RdMockupImageEventListener()
        {
            images_arrived = 0;
        }

        virtual bool onImageArrived( RdImageManager * manager )
        {
            stored_image = manager->getImage();
            images_arrived++;
        }

        int getImagesArrived() { return images_arrived; }
        void resetImagesArrived() { images_arrived = 0; }

        RdImage getStoredImage() { return stored_image; }


    private:
        int images_arrived;
        RdImage stored_image;
};


class RdYarpImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            RdYarpImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager(RdYarpImageManager::id);
        }

        virtual void TearDown()
        {
            RdImageManager::destroyImageManager();
        }


    protected:
        RdImageManager * imageManager;
};


TEST_F( RdYarpImageManagerTest, RdYarpImageManagerNotificationWorks)
{
    //-- Create a mockup listener
    RdMockupImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Configure the camera port
    ASSERT_TRUE(imageManager->configure("port", "/grabber"));

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Wait for a image to arrive (~20ms ?? )
    usleep(20000);

    //-- Check that a image arrived
    EXPECT_LE( 1, listener.getImagesArrived() );

    //-- Dettach the listener
    EXPECT_TRUE(imageManager->removeImageEventListeners());
}
