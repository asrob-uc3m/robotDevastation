#include "gtest/gtest.h"

#include "RdImageManager.hpp"


using namespace rd;

class BasicImageManager : public RdImageManager
{
    public:
        virtual bool start() {}
        virtual bool stop() {}
        virtual yarp::sig::ImageOf<yarp::sig::PixelRgb> getImage() { return yarp::sig::ImageOf<yarp::sig::PixelRgb>();}

        static const std::string id;

        BasicImageManager() {
            Register(this, id);
        }

        ~BasicImageManager() {
            RD_INFO("Hey, I'm a destructor!\n");
        }
};

const std::string BasicImageManager::id = "TEST_MANAGER";

class RdImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp() {
            imageManager = new BasicImageManager();
        }

        virtual void TearDown()
        {
            RdImageManager::destroyImageManager();
        }


    protected:
        RdImageManager * imageManager;
};

TEST_F( RdImageManagerTest, RdImageManagerIsSingleton)
{
    RdImageManager * manager2 = NULL;
    manager2 = RdImageManager::getImageManager();

    ASSERT_NE((RdImageManager *)NULL, imageManager);
    ASSERT_NE((RdImageManager *)NULL, manager2);
    ASSERT_EQ(imageManager, manager2);
}

