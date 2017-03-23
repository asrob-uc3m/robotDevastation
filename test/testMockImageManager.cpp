#include "gtest/gtest.h"
#include <yarp/os/Property.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Network.h>
#include <yarp/sig/all.h>
#include <yarp/os/ResourceFinder.h>

#include "ImageManager.hpp"
#include "MockImageManager.hpp"
#include "MockImageEventListener.hpp"

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockupImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Find the real path to the resources with ResourceFinder
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");
            image_filename = rf.findFileByName(image_filename_raw);

            MockupImageManager::RegisterManager();
            imageManager = ImageManager::getImageManager(MockupImageManager::id);
        }

        virtual void TearDown()
        {
            ImageManager::destroyImageManager();
        }

        static const std::string image_filename_raw;
        std::string image_filename;

    protected:
        ImageManager * imageManager;

};

const std::string MockupImageManagerTest::image_filename_raw = "../images/test_frame.ppm";

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the webcam in a port so that the imageManager can connect to it

class MockupImageManagerEnvironment : public testing::Environment
{
    public:
        MockupImageManagerEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();
        }

        virtual void TearDown()
        {
            yarp::os::Network::fini();
        }

    private:
        int argc;
        char ** argv;
};


//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------

TEST_F(MockupImageManagerTest, MockupImageManagerNotificationWorks)
{
    //-- Create a mockup listener
    MockupImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Check that an image didn't arrive
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Load test image
    Image test_image;
    ASSERT_TRUE(yarp::sig::file::read(test_image, MockupImageManagerTest::image_filename));

    //-- Check that an image didn't arrive
    EXPECT_FALSE(((MockupImageManager *) imageManager)->receiveImage(test_image));
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Enable manager
    ASSERT_TRUE(imageManager->setEnabled(true));

    //-- Send test image to mockup manager
    ASSERT_TRUE(((MockupImageManager *) imageManager)->receiveImage(test_image));

    //-- Check that the correct image arrived
    EXPECT_EQ(1, listener.getImagesArrived());
    Image received_image = listener.getStoredImage();

    ASSERT_EQ(received_image.width(), test_image.width());
    ASSERT_EQ(received_image.height(), test_image.height());
    for (int i = 0 ; i < received_image.width(); i++)
        for (int j = 0; j < received_image.height(); j++)
        {
            EXPECT_EQ(received_image(i, j).r, test_image(i, j).r);
            EXPECT_EQ(received_image(i, j).g, test_image(i, j).g);
            EXPECT_EQ(received_image(i, j).b, test_image(i, j).b);
        }

    //-- Dettach the listener
    EXPECT_TRUE(imageManager->removeImageEventListeners());
    ASSERT_TRUE(imageManager->stop());
    ASSERT_TRUE(imageManager->isStopped());
    ASSERT_FALSE(((MockupImageManager *) imageManager)->isEnabled());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new MockupImageManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
