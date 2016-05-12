#include "gtest/gtest.h"
#include <yarp/sig/all.h>

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
            imageManager->addImageEventListener(&processor);

            mentalMap = RdMentalMap::getMentalMap();

            //-- Load test image
            yarp::sig::file::read(test_image, image_filename);

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

TEST_F(RdProcessorImageEventListenerTest, TargetDetectionWorks)
{


}
