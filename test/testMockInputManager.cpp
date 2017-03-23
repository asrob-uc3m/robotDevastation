#include "gtest/gtest.h"
#include "MockInputManager.hpp"
#include "MockInputEventListener.hpp"

using namespace rd;

//-- Class for the setup of each test
//--------------------------------------------------------------------------------------
class MockInputManagerTest : public testing::Test
{
    public:
        virtual void SetUp()
        {
            ASSERT_TRUE(MockInputManager::RegisterManager());
            inputManager = (MockInputManager *) MockInputManager::getInputManager(MockInputManager::id);
            ASSERT_TRUE(inputManager);
        }

        virtual void TearDown()
        {
            ASSERT_TRUE(MockInputManager::destroyInputManager());
        }


    protected:
        MockInputManager * inputManager;
};


//-- Things that are being tested
//-----------------------------------------------------------------------------------------------------
TEST_F(MockInputManagerTest, SingleControlKeyPressSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));
    ASSERT_EQ(1, inputManager->getNumListeners());

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    Key enter_key(Key::KEY_ENTER);
    ASSERT_TRUE(inputManager->sendKeyPress(enter_key));

    //-- Check if keypress was received
    ASSERT_EQ(1,listener.getNumKeyDownPresses());
    ASSERT_EQ(1,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<Key> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<Key> keys_up_received = listener.getStoredKeyDownPresses();
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

TEST_F(MockInputManagerTest, SinglePrintableKeyPressSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    Key d_key('d');
    ASSERT_TRUE(inputManager->sendKeyPress(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(1,listener.getNumKeyDownPresses());
    ASSERT_EQ(1,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<Key> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<Key> keys_up_received = listener.getStoredKeyDownPresses();
    ASSERT_EQ(1,keys_down_received.size());
    ASSERT_EQ(1,keys_up_received.size());

    ASSERT_EQ(d_key.getChar(), keys_down_received[0].getChar());
    ASSERT_EQ(d_key.getChar(), keys_up_received[0].getChar());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}

TEST_F(MockInputManagerTest, SeveralControlKeyPressesSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    Key enter_key(Key::KEY_ENTER);
    Key arrow_key(Key::KEY_ARROW_DOWN);
    ASSERT_TRUE(inputManager->sendKeyPress(enter_key));
    ASSERT_TRUE(inputManager->sendKeyPress(arrow_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(2,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<Key> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<Key> keys_up_received = listener.getStoredKeyDownPresses();
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

TEST_F(MockInputManagerTest, SeveralPrintableKeyPressesSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a control keypress
    Key r_key('r');
    Key d_key('d');
    ASSERT_TRUE(inputManager->sendKeyPress(r_key));
    ASSERT_TRUE(inputManager->sendKeyPress(d_key));

    //-- Check if keypress was received
    ASSERT_EQ(2,listener.getNumKeyDownPresses());
    ASSERT_EQ(2,listener.getNumKeyUpPresses());

    //-- Check if keypress received is correct
    std::vector<Key> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<Key> keys_up_received = listener.getStoredKeyDownPresses();
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

TEST_F(MockInputManagerTest, KeyUpAndKeyDownSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a key down
    Key enter_key(Key::KEY_ENTER);
    Key d_key('d');
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
    std::vector<Key> keys_down_received = listener.getStoredKeyDownPresses();
    std::vector<Key> keys_up_received = listener.getStoredKeyDownPresses();
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

TEST_F(MockInputManagerTest, WindowEventSentAndReceivedCorrectly)
{
    MockInputEventListener listener;
    InputEventListener * plistener = (InputEventListener *) &listener;
    ASSERT_TRUE(((InputManager*)inputManager)->addInputEventListener(plistener));
    ASSERT_EQ(1, inputManager->getNumListeners());

    ASSERT_TRUE(inputManager->start());
    ASSERT_FALSE(inputManager->isStopped());

    //-- Fake a window event
    WindowEvent window_event(WindowEvent::WINDOW_CLOSE);
    ASSERT_TRUE(inputManager->sendWindowEvent(window_event));

    //-- Check if window event was received
    ASSERT_EQ(1,listener.getNumWindowEvents());

    //-- Check if window event received is correct
    std::vector<WindowEvent> window_events_received = listener.getStoredWindowEvents();
    ASSERT_EQ(1,window_events_received.size());

    ASSERT_EQ(window_event.getEvent(), window_events_received[0].getEvent());

    //-- Cleanup
    ASSERT_TRUE(inputManager->removeInputEventListeners());
    ASSERT_EQ(0, inputManager->getNumListeners());
    ASSERT_TRUE(inputManager->stop());
    ASSERT_TRUE(inputManager->isStopped());
}
