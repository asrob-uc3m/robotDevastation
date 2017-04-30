// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MOCK_INPUT_MANAGER_HPP__
#define __RD_MOCK_INPUT_MANAGER_HPP__

#include <string>

#include "InputManager.hpp"
#include "Key.hpp"
#include "WindowEvent.hpp"

namespace rd{

/**
 * @ingroup InputLib
 *
 * @brief Allows to simulate input from a user programatically (mainly for test purposes)
 *
 */
class MockInputManager : public InputManager
{
    public:
        //------------------------------ Testing Interface ------------------------------------------------------------//
        bool sendKeyPress(const Key & key);
        bool sendKeyUp(const Key & key);
        bool sendKeyDown(const Key & key);
        bool sendWindowEvent(const WindowEvent & event);

        //------------------------------ Construction & destruction ---------------------------------------------------//
        virtual ~MockInputManager();

        /**
         * @brief Register this manager in the InputManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        virtual bool start();
        virtual bool stop();
        virtual bool isStopped() const;

        //------------------------------ Listeners---------------------------------------------------------------------//
        int getNumListeners();

   private:
        /**
        * @brief Constructor
        *
        * Constructor for this class is private, since the singleton can only be instantiated once,
        * and the instantiation is done at the RegisterManager() method.
        */
        MockInputManager();

        MockInputManager(const MockInputManager &);
        MockInputManager & operator=(const MockInputManager &);

        //! @brief Reference to this manager (unique instance)
        static MockInputManager * uniqueInstance;

        bool stopped;
};
}
#endif // __RD_MOCK_INPUT_MANAGER_HPP__
