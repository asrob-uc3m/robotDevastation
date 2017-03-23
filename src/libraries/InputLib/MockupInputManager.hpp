// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_INPUT_MANAGER_HPP__
#define __MOCKUP_INPUT_MANAGER_HPP__

#include "InputManager.hpp"

namespace rd{

/**
 * @ingroup RdInputLib
 *
 * @brief Allows to simulate input from a user programatically (mainly for test purposes)
 *
 */
class MockupInputManager : public InputManager
{
    public:
        //------------------------------ Testing Interface ------------------------------------------------------------//
        bool sendKeyPress(const Key & key);
        bool sendKeyUp(const Key & key);
        bool sendKeyDown(const Key & key);
        bool sendWindowEvent(const WindowEvent & event);

        //------------------------------ Construction & destruction ---------------------------------------------------//
        MockupInputManager();
        ~MockupInputManager();

        /**
         * @brief Register this manager in the RdInputManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped();

        //------------------------------ Listeners---------------------------------------------------------------------//
        int getNumListeners();

   private:
        //! @brief Reference to this manager (unique instance)
        static MockupInputManager * uniqueInstance;

        bool stopped;
};
}
#endif // __MOCKUP_INPUT_MANAGER_HPP__
