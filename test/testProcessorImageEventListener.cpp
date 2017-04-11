#include "gtest/gtest.h"
#include <yarp/sig/all.h>
#include <yarp/os/Time.h>
#include <yarp/os/ResourceFinder.h>

#include "ImageManager.hpp"
#include "MockImageManager.hpp"
#include "MockImageEventListener.hpp"
#include "MentalMap.hpp"
#include "ProcessorImageEventListener.hpp"

using namespace rd;


//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class ProcessorImageEventListenerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            //-- Configure Resourcefinder to ind the real path to the resources
            yarp::os::ResourceFinder rf;
            rf.setDefaultContext("robotDevastation");
            rf.setDefaultConfigFile("robotDevastation.ini");

            MockImageManager::RegisterManager();
            imageManager = ImageManager::getImageManager(MockImageManager::id);
            ASSERT_NE((ImageManager*)NULL, imageManager);
            imageManager->addImageEventListener(&processor);

            mentalMap = MentalMap::getMentalMap();
            ASSERT_NE((MentalMap*)NULL, mentalMap);

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
            ImageManager::destroyImageManager();
            imageManager = NULL;

            MentalMap::destroyMentalMap();
            mentalMap = NULL;
        }

        static const std::string image_filename;
        static const std::string bad_image_filename;

    protected:
        ImageManager * imageManager;
        MentalMap * mentalMap;
        ProcessorImageEventListener processor;

        Image test_image;
        Image bad_image;
};

const std::string ProcessorImageEventListenerTest::image_filename = "../images/test_target.ppm";
const std::string ProcessorImageEventListenerTest::bad_image_filename = "../images/test_target_bad.ppm";

void compare_targets(Target target1, Target target2, int threshold = 20)
{
    ASSERT_EQ(target1.getPlayerId(), target2.getPlayerId());
    ASSERT_NEAR(target1.getPos().getX(), target2.getPos().getX(), threshold);
    ASSERT_NEAR(target1.getPos().getY(), target2.getPos().getY(), threshold);
    ASSERT_NEAR(target1.getDimensions().getX(), target2.getDimensions().getX(), threshold);
    ASSERT_NEAR(target1.getDimensions().getY(), target2.getDimensions().getY(), threshold);
}

TEST_F(ProcessorImageEventListenerTest, TargetDetectionWorks)
{
    //-- Expected targets:
    Target target1(1, Vector2d(200, 200), Vector2d(220,220));
    Target target2(2, Vector2d(500, 100), Vector2d(150, 150));

    //-- Send image to image manager
    ASSERT_TRUE(imageManager->start());
    ASSERT_TRUE(imageManager->setEnabled(true));
    ASSERT_TRUE(((MockImageManager *)imageManager)->receiveImage(test_image));
    yarp::os::Time::delay(0.5);

    //-- Check detected targets:
    std::vector<Target> targets_detected = ProcessorImageEventListenerTest::mentalMap->getTargets();
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

TEST_F(ProcessorImageEventListenerTest, BadQRsAreIgnored)
{
    //-- Send image to image manager
    ASSERT_TRUE(imageManager->start());
    ASSERT_TRUE(imageManager->setEnabled(true));
    ASSERT_TRUE(((MockImageManager *)imageManager)->receiveImage(bad_image));
    yarp::os::Time::delay(0.5);

    //-- Check detected targets:
    std::vector<Target> targets_detected = ProcessorImageEventListenerTest::mentalMap->getTargets();
    ASSERT_EQ(0, targets_detected.size());
}

//--- Main -------------------------------------------------------------------------------------------
int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
