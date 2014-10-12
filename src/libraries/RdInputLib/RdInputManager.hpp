// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_MANAGER_HPP__
#define __RD_INPUT_MANAGER_HPP__

#include <SDL/SDL.h>

#include <vector>

#include "RdUtils.hpp"
#include "RdInputEventListener.hpp"
#include "RdKeyboardManager.hpp"


namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdInputManagerLib
 *
 * @brief The RdInputManagerLib library contains the \ref RdInputManager base class and derived classes.
 */

/**
 * @ingroup RdInputManagerLib
 *
 * @brief A base class for Input Managers.
 *
 */
class RdInputManager
{
    public:
        static RdInputManager * getInputManager();
        static bool destroyInputManager();

        ~RdInputManager();

        bool start();

        bool addInputEventListener( RdInputEventListener * listener );
        bool removeInputEventListeners();

    private:
        RdInputManager();

        static RdInputManager * inputManagerInstance;
        RdKeyboardManager * keyboardManager;
        std::vector<RdInputEventListener *> listeners;

};

}

#endif //-- __RD_INPUT_MANAGER_HPP__
