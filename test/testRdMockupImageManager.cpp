#include "gtest/gtest.h"
#include <yarp/os/Property.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Network.h>
#include <yarp/sig/all.h>

#include "RdImageManager.hpp"
#include "RdMockupImageManager.hpp"
#include "RdMockupImageEventListener.hpp"

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RdMockupImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            RdMockupImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager(RdMockupImageManager::id);
        }

        virtual void TearDown()
        {
            RdImageManager::destroyImageManager();
        }

        static const std::string image_filename;

    protected:
        RdImageManager * imageManager;

};

const std::string RdMockupImageManagerTest::image_filename = "../../share/images/test_frame.ppm";

//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the webcam in a port so that the imageManager can connect to it

class RdMockupImageManagerEnvironment : public testing::Environment
{
    public:
        RdMockupImageManagerEnvironment(int argc, char ** argv)
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

TEST_F(RdMockupImageManagerTest, RdMockupImageManagerNotificationWorks)
{
    //-- Create a mockup listener
    RdMockupImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Check that an image didn't arrive
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Load test image
    RdImage test_image;
    ASSERT_TRUE(yarp::sig::file::read(test_image, RdMockupImageManagerTest::image_filename));

    //-- Check that an image didn't arrive
    EXPECT_FALSE(((RdMockupImageManager *) imageManager)->receiveImage(test_image));
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Enable manager
    ASSERT_TRUE(imageManager->setEnabled(true));

    //-- Send test image to mockup manager
    ASSERT_TRUE(((RdMockupImageManager *) imageManager)->receiveImage(test_image));

    //-- Check that the correct image arrived
    EXPECT_EQ(1, listener.getImagesArrived());
    RdImage received_image = listener.getStoredImage();

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
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RdMockupImageManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
