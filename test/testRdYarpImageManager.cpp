#include "gtest/gtest.h"
#include <yarp/os/Property.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Network.h>

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
 *  @todo Move this class to RdImageLib, set it to be compiled only with the tests
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

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
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


//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the webcam in a port so that the imageManager can connect to it

class RdYarpImageManagerEnvironment : public testing::Environment
{
    public:
        RdYarpImageManagerEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::Network::init();
            yarp::os::Network::runNameServer(argc, argv);

            //-- Setup yarp camera
            yarp::os::Property cameraOptions;
            cameraOptions.put("device","grabber");
            cameraOptions.put("subdevice","opencv_grabber");
            cameraOptions.put("name",camera_port_name);
            cameraDevice.open(cameraOptions);

            if (!cameraDevice.isValid())
            {
                RD_ERROR("Camera class instantiation not worked.\n");
                return;
            }

        }

        virtual void TearDown()
        {
            cameraDevice.close();

            yarp::os::Network::fini();
        }

        static const std::string camera_port_name;

    private:
        int argc;
        char ** argv;
        yarp::dev::PolyDriver cameraDevice;
};

const std::string RdYarpImageManagerEnvironment::camera_port_name = "/grabber";



//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------

TEST_F( RdYarpImageManagerTest, RdYarpImageManagerNotificationWorks)
{
    //-- Create a mockup listener
    RdMockupImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Configure the camera port
    ASSERT_TRUE(imageManager->configure("remote_img_port", RdYarpImageManagerEnvironment::camera_port_name ));
    ASSERT_TRUE(imageManager->configure("local_img_port", "/asdf" )); //-- Name given by me

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Wait for a image to arrive
    usleep(1e6);

    //-- Check that a image arrived
    EXPECT_LE( 1, listener.getImagesArrived() );

    //-- Dettach the listener
    EXPECT_TRUE(imageManager->removeImageEventListeners());
}

TEST_F( RdYarpImageManagerTest, RdYarpImageManagerReadsImage)
{
    //-- Create a mockup listener
    RdMockupImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Configure the camera port
    ASSERT_TRUE(imageManager->configure("remote_img_port", RdYarpImageManagerEnvironment::camera_port_name ));
    ASSERT_TRUE(imageManager->configure("local_img_port", "/asdf" )); //-- Name given by me

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Wait for a image to arrive
    usleep(1e6);

    //-- Check that a image arrived
    ASSERT_LE( 1, listener.getImagesArrived() );

    //-- Read image
    RdImage image = imageManager->getImage();
    EXPECT_NE( 0, image.width());
    EXPECT_NE( 0, image.height());

    //-- Dettach the listener
    EXPECT_TRUE(imageManager->removeImageEventListeners());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new RdYarpImageManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
