#include <RdInputManager.hpp>
#include <RdInputEventListener.hpp>
#include <RdAudioManager.hpp>

using namespace rd;

class RdTestEventListener : public RdInputEventListener
{
    public:
        RdTestEventListener()
        {
            audioManager = RdAudioManager::getAudioManager();
            audioManager->load( "../../share/sounds/01_milshot.wav", "shoot", RdAudioManager::FX);
        }

        ~RdTestEventListener()
        {
            RdAudioManager::destroyAudioManager();
        }

        virtual bool onKeyPressed(RdKey k)
        {
            audioManager->playSound("shoot", false);
        }

    private:
        RdAudioManager * audioManager;
};

int main(void)
{
    RdInputManager * inputManager = RdInputManager::getInputManager();
    RdTestEventListener listener;

    inputManager->addInputEventListener(&listener);
    inputManager->start();

    //-- There must be a better way of doing this than this xD
    while(true);

    RdInputManager::destroyInputManager();
}
