// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_YARP_STATE_DIRECTOR_HPP__
#define __RD_YARP_STATE_DIRECTOR_HPP__

#include <yarp/os/PeriodicThread.h>

#include "StateDirector.hpp"
#include "State.hpp"

namespace rd {

/**
 * @ingroup StateMachineLib
 *
 * @brief StateDirector based on YARP, creates a separate thread
 */
class YarpStateDirector : public StateDirector, private yarp::os::PeriodicThread
{
public:
    YarpStateDirector(State * state);

    /**
     * @brief Function that starts the State execution using YARP
     *
     * It launches a yarp::os::PeriodicThread thread after State.setup().
     */
    virtual bool Start();
    virtual bool Stop();

    //! @brief Function called periodically by YARP, that calls the State.loop() method
    void run();

    //! @brief Period of the calls to the State loop() function
    static const int DEFAULT_RATE_MS;

};

}
#endif // __RD_YARP_STATE_DIRECTOR_HPP__
