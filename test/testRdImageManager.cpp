#include "gtest/gtest.h"

#include "RdImageManager.hpp"


using namespace rd;


/**
 * @brief Dummy image manager used for testing
 *
 * The required pure virtual members are left with their function body empty.
 *
 * This is the minimum implementation possible for the RdImageManager interface.
 *
 */
class BasicImageManager : public RdImageManager
{
    public:
        virtual bool start() {}
        virtual bool stop() {}
        virtual RdImage getImage()
        {
            return RdImage();
        }

        //-- This function is needed to register this Manager on the RdImageManager registry
        //-- It ensures that only one manager of this type is created (unique instance)
        static bool RegisterManager()
        {
            if (uniqueInstance == NULL)
            {
                uniqueInstance = new BasicImageManager();
            }

            return Register( uniqueInstance, id);
        }

        //-- The local static reference must be reseted after destroying this manager
        ~BasicImageManager()
        {
            RD_INFO("Hey, I'm a destructor!\n");
            uniqueInstance = NULL;
        }

        //-- This string identifies this manager
        static const std::string id;

    private:
        //-- Store a reference to this manager (unique instance)
        static BasicImageManager * uniqueInstance;
};


BasicImageManager * BasicImageManager::uniqueInstance = NULL;
const std::string BasicImageManager::id = "TEST_MANAGER";


class RdImageManagerTest : public testing::Test
{
    public:
        virtual void SetUp() {
            BasicImageManager::RegisterManager();
            imageManager = RdImageManager::getImageManager();
        }

        virtual void TearDown()
        {
            RdImageManager::destroyImageManager();
        }


    protected:
        RdImageManager * imageManager;
};


TEST_F( RdImageManagerTest, RdImageManagerRetrievedWithoutId)
{
    RdImageManager * manager2 = NULL;
    manager2 = RdImageManager::getImageManager();

    ASSERT_NE((RdImageManager *)NULL, imageManager);
    ASSERT_NE((RdImageManager *)NULL, manager2);
    ASSERT_EQ(imageManager, manager2);
}


TEST_F( RdImageManagerTest, RdImageManagerRetrievedWithId)
{
    RdImageManager * manager2 = NULL;
    manager2 = RdImageManager::getImageManager(BasicImageManager::id);

    ASSERT_NE((RdImageManager *)NULL, imageManager);
    ASSERT_NE((RdImageManager *)NULL, manager2);
    ASSERT_EQ(imageManager, manager2);
}
