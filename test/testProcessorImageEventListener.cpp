#include "gtest/gtest.h"
#include <yarp/sig/all.h>
#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

#include "ImageManager.hpp"
#include "MockupImageManager.hpp"
#include "MockupImageEventListener.hpp"
#include "MentalMap.hpp"
#include "ProcessorImageEventListener.hpp"

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RdProcessorImageEventListenerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Configure Resourcefinder to ind the real path to the resources
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");

            RdMockupImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager(RdMockupImageManager::id);
            ASSERT_NE((RdImageManager*)NULL, imageManager);
            imageManager->addImageEventListener(&processor);

            mentalMap = RdMentalMap::getMentalMap();
            ASSERT_NE((RdMentalMap*)NULL, mentalMap);

            //-- Load test images
            yarp::sig::file::read(test_image, rf.findFileByName(image_filename));
            ASSERT_NE(0, test_image.width());
            ASSERT_NE(0, test_image.height());

            yarp::sig::file::read(bad_image, rf.findFileByName(bad_image_filename));
            ASSERT_NE(0, test_image.width());
            ASSERT_NE(0, test_image.height());
        }

        virtual void TearDown()
        {
            RdImageManager::destroyImageManager();
            imageManager = NULL;

            RdMentalMap::destroyMentalMap();
            mentalMap = NULL;
        }

        static const std::string image_filename;
        static const std::string bad_image_filename;

    protected:
        RdImageManager * imageManager;
        RdMentalMap * mentalMap;
        RdProcessorImageEventListener processor;

        RdImage test_image;
        RdImage bad_image;
};

const std::string RdProcessorImageEventListenerTest::image_filename = "../images/test_target.ppm";
const std::string RdProcessorImageEventListenerTest::bad_image_filename = "../images/test_target_bad.ppm";

void compare_targets(RdTarget target1, RdTarget target2, int threshold = 20)
{
    ASSERT_EQ(target1.getPlayerId(), target2.getPlayerId());
    ASSERT_NEAR(target1.getPos().x, target2.getPos().x, threshold);
    ASSERT_NEAR(target1.getPos().y, target2.getPos().y, threshold);
    ASSERT_NEAR(target1.getDimensions().x, target2.getDimensions().x, threshold);
    ASSERT_NEAR(target1.getDimensions().y, target2.getDimensions().y, threshold);
}

TEST_F(RdProcessorImageEventListenerTest, TargetDetectionWorks)
{
    //-- Expected targets:
    RdTarget target1(1, RdVector2d(200, 200), RdVector2d(220,220));
    RdTarget target2(2, RdVector2d(500, 100), RdVector2d(150, 150));

    //-- Send image to image manager
    ASSERT_TRUE(imageManager->start());
    ASSERT_TRUE(imageManager->setEnabled(true));
    ASSERT_TRUE(((RdMockupImageManager *)imageManager)->receiveImage(test_image));
    yarp::os::Time::delay(0.5);

    //-- Check detected targets:
    std::vector<RdTarget> targets_detected = RdProcessorImageEventListenerTest::mentalMap->getTargets();
    ASSERT_EQ(2, targets_detected.size());

    if (targets_detected[0].getPlayerId() == target1.getPlayerId())
    {
        compare_targets(target1, targets_detected[0]);
        compare_targets(target2, targets_detected[1]);
    }
    else
    {
        compare_targets(target1, targets_detected[1]);
        compare_targets(target2, targets_detected[0]);
    }

}

TEST_F(RdProcessorImageEventListenerTest, BadQRsAreIgnored)
{
    //-- Send image to image manager
    ASSERT_TRUE(imageManager->start());
    ASSERT_TRUE(imageManager->setEnabled(true));
    ASSERT_TRUE(((RdMockupImageManager *)imageManager)->receiveImage(bad_image));
    yarp::os::Time::delay(0.5);

    //-- Check detected targets:
    std::vector<RdTarget> targets_detected = RdProcessorImageEventListenerTest::mentalMap->getTargets();
    ASSERT_EQ(0, targets_detected.size());
}
