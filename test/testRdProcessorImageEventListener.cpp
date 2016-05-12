#include "gtest/gtest.h"
#include <yarp/sig/all.h>
#include <yarp/os/Time.h>

#include "RdImageManager.hpp"
#include "RdMockupImageManager.hpp"
#include "RdMockupImageEventListener.hpp"
#include "RdMentalMap.hpp"
#include "RdProcessorImageEventListener.hpp"

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class RdProcessorImageEventListenerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            RdMockupImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager(RdMockupImageManager::id);
            ASSERT_NE((RdImageManager*)NULL, imageManager);
            imageManager->addImageEventListener(&processor);

            mentalMap = RdMentalMap::getMentalMap();
            ASSERT_NE((RdMentalMap*)NULL, mentalMap);

            //-- Load test image
            yarp::sig::file::read(test_image, image_filename);
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

    protected:
        RdImageManager * imageManager;
        RdMentalMap * mentalMap;
        RdProcessorImageEventListener processor;

        RdImage test_image;
};

const std::string RdProcessorImageEventListenerTest::image_filename = "../../share/images/test_target.ppm";

void compare_targets(RdTarget target1, RdTarget target2)
{
    ASSERT_EQ(target1.getPlayerId(), target2.getPlayerId());
    ASSERT_NEAR(target1.getPos().x, target2.getPos().x, 10);
    ASSERT_NEAR(target1.getPos().y, target2.getPos().y, 10);
    ASSERT_NEAR(target1.getDimensions().x, target2.getDimensions().x, 10);
    ASSERT_NEAR(target1.getDimensions().y, target2.getDimensions().y, 10);
}

TEST_F(RdProcessorImageEventListenerTest, TargetDetectionWorks)
{
    //-- Expected targets:
    RdTarget target1(1, RdVector2d(200, 200), RdVector2d(220,220));
    RdTarget target2(2, RdVector2d(500, 100), RdVector2d(150, 150));

    //-- Send image to image manager
    ASSERT_TRUE(imageManager->start());
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
