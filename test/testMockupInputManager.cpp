#include "gtest/gtest.h"
#include "MockupInputManager.hpp"
#include "MockupInputEventListener.hpp"

using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockupInputManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            ASSERT_TRUE(MockupInputManager::RegisterManager());
            inputManager = (MockupInputManager *) MockupInputManager::getInputManager(MockupInputManager::id);
            ASSERT_TRUE(inputManager);
        }

        virtual void TearDown()
        {
            ASSERT_TRUE(MockupInputManager::destroyInputManager());
        }


    protected:
        MockupInputManager * inputManager;
};


//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(MockupInputManagerTest, SingleControlKeyPressSentAndReceivedCorrectly)
{
    MockupInputEventListener listener;
    RdInputEventListener * plistener = (RdInputEventListener *) &listener;
    ASSERT_TRUE(((RdInputManager*)inputManager)->addInputEventListener(plistener));
    ASSERT_EQ(1, inputManager->getNumListeners());

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    RdKey enter_key(RdKey::KEY_ENTER);
    ASSERT_TRUE(inputManager->sendKeyPress(enter_key));

    //-- Check if keypress was received
    ASSERT_EQ(1,listener.getNumKeyDownPresses());
    ASSERT_EQ(1,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<RdKey> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<RdKey> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(1,keys_down_received.size());
    ASSERT_EQ(1,keys_up_received.size());

    ASSERT_EQ(enter_key.getValue(), keys_down_received[0].getValue());
    ASSERT_EQ(enter_key.getValue(), keys_up_received[0].getValue());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_EQ(0, inputManager->getNumListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}

TEST_F(MockupInputManagerTest, SinglePrintableKeyPressSentAndReceivedCorrectly)
{
    MockupInputEventListener listener;
    RdInputEventListener * plistener = (RdInputEventListener *) &listener;
    ASSERT_TRUE(((RdInputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    RdKey d_key('d');
    ASSERT_TRUE(inputManager->sendKeyPress(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(1,listener.getNumKeyDownPresses());
    ASSERT_EQ(1,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<RdKey> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<RdKey> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(1,keys_down_received.size());
    ASSERT_EQ(1,keys_up_received.size());

    ASSERT_EQ(d_key.getChar(), keys_down_received[0].getChar());
    ASSERT_EQ(d_key.getChar(), keys_up_received[0].getChar());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}

TEST_F(MockupInputManagerTest, SeveralControlKeyPressesSentAndReceivedCorrectly)
{
    MockupInputEventListener listener;
    RdInputEventListener * plistener = (RdInputEventListener *) &listener;
    ASSERT_TRUE(((RdInputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    RdKey enter_key(RdKey::KEY_ENTER);
    RdKey arrow_key(RdKey::KEY_ARROW_DOWN);
    ASSERT_TRUE(inputManager->sendKeyPress(enter_key));
    ASSERT_TRUE(inputManager->sendKeyPress(arrow_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(2,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<RdKey> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<RdKey> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(2,keys_down_received.size());
    ASSERT_EQ(2,keys_up_received.size());

    ASSERT_EQ(enter_key.getValue(), keys_down_received[0].getValue());
    ASSERT_EQ(enter_key.getValue(), keys_up_received[0].getValue());
    ASSERT_EQ(arrow_key.getValue(), keys_down_received[1].getValue());
    ASSERT_EQ(arrow_key.getValue(), keys_up_received[1].getValue());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}

TEST_F(MockupInputManagerTest, SeveralPrintableKeyPressesSentAndReceivedCorrectly)
{
    MockupInputEventListener listener;
    RdInputEventListener * plistener = (RdInputEventListener *) &listener;
    ASSERT_TRUE(((RdInputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    RdKey r_key('r');
    RdKey d_key('d');
    ASSERT_TRUE(inputManager->sendKeyPress(r_key));
    ASSERT_TRUE(inputManager->sendKeyPress(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(2,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<RdKey> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<RdKey> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(2,keys_down_received.size());
    ASSERT_EQ(2,keys_up_received.size());

    ASSERT_EQ(r_key.getChar(), keys_down_received[0].getChar());
    ASSERT_EQ(r_key.getChar(), keys_up_received[0].getChar());
    ASSERT_EQ(d_key.getChar(), keys_down_received[1].getChar());
    ASSERT_EQ(d_key.getChar(), keys_up_received[1].getChar());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}

TEST_F(MockupInputManagerTest, KeyUpAndKeyDownSentAndReceivedCorrectly)
{
    MockupInputEventListener listener;
    RdInputEventListener * plistener = (RdInputEventListener *) &listener;
    ASSERT_TRUE(((RdInputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a key down
    RdKey enter_key(RdKey::KEY_ENTER);
    RdKey d_key('d');
    ASSERT_TRUE(inputManager->sendKeyDown(enter_key));
    ASSERT_TRUE(inputManager->sendKeyDown(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(0,listener.getNumKeyUpPresses());

    //-- Fake a key up
    ASSERT_TRUE(inputManager->sendKeyUp(enter_key));
    ASSERT_TRUE(inputManager->sendKeyUp(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(2,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<RdKey> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<RdKey> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(2,keys_down_received.size());
    ASSERT_EQ(2,keys_up_received.size());

    ASSERT_EQ(enter_key.getValue(), keys_down_received[0].getValue());
    ASSERT_EQ(enter_key.getValue(), keys_up_received[0].getValue());
    ASSERT_EQ(d_key.getChar(), keys_down_received[1].getChar());
    ASSERT_EQ(d_key.getChar(), keys_up_received[1].getChar());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}
