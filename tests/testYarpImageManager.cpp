// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "gtest/gtest.h"
#include <yarp/os/Property.h>
#include <yarp/dev/PolyDriver.h>
#include <yarp/os/Network.h>
#include <yarp/os/Time.h>

#include "ImageManager.hpp"
#include "YarpImageManager.hpp"
#include "MockImageEventListener.hpp"

#include <ColorDebug.h>

namespace rd
{

namespace test
{

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class YarpImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            YarpImageManager::RegisterManager();
            imageManager = ImageManager::getImageManager(YarpImageManager::id);
        }

        virtual void TearDown()
        {
            ImageManager::destroyImageManager();
        }


    protected:
        ImageManager * imageManager;
};


//-- Class for the setup of the enviroment for all the tests
//----------------------------------------------------------------------------------------
//-- This opens the webcam in a port so that the imageManager can connect to it

class YarpImageManagerEnvironment : public testing::Environment
{
    public:
        YarpImageManagerEnvironment(int argc, char ** argv)
        {
            this->argc = argc;
            this->argv = argv;
        }

        virtual void SetUp()
        {
            //-- Init yarp network & server
            yarp::os::NetworkBase::setLocalMode(true);
            yarp::os::Network::init();

            //-- Setup yarp camera
            yarp::os::Property cameraOptions;
            cameraOptions.put("device","grabber");
            cameraOptions.put("subdevice","opencv_grabber");
            cameraOptions.put("name",camera_port_name);
            cameraDevice.open(cameraOptions);

            if (!cameraDevice.isValid())
            {
                CD_ERROR("Camera class instantiation not worked.\n");
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

const std::string YarpImageManagerEnvironment::camera_port_name = "/grabber";



//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------

TEST_F( YarpImageManagerTest, YarpImageManagerWorks)
{
    //-- Create a mock listener
    MockImageEventListener listener;

    //-- Add the listener to the manager
    ASSERT_TRUE(imageManager->addImageEventListener(&listener));

    //-- Configure the camera port
    ASSERT_TRUE(imageManager->configure("remote_img_port", YarpImageManagerEnvironment::camera_port_name));
    ASSERT_TRUE(imageManager->configure("local_img_port", "/asdf")); //-- Name given by me

    //-- Start the imageManager
    listener.resetImagesArrived();
    ASSERT_TRUE(imageManager->start());

    //-- Wait for a image to arrive
    yarp::os::Time::delay(1);

    //-- Check that a image didn't arrive
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Enable and wait for a image to arrive
    ASSERT_TRUE(imageManager->setEnabled(true));
    yarp::os::Time::delay(1);

    //-- Check that a image arrived
    EXPECT_LE( 1, listener.getImagesArrived());

    //-- Check that disabling again works
    ASSERT_TRUE(imageManager->setEnabled(false));
    listener.resetImagesArrived();
    yarp::os::Time::delay(1);
    EXPECT_EQ(0, listener.getImagesArrived());

    //-- Dettach the listener
    EXPECT_TRUE(imageManager->removeImageEventListeners());

    //-- Stop the manager
    ASSERT_TRUE(imageManager->stop());
}

}  // namespace test

}  // namespace rd

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);
  testing::Environment* env = testing::AddGlobalTestEnvironment(new rd::test::YarpImageManagerEnvironment(argc, argv));
  return RUN_ALL_TESTS();
}
