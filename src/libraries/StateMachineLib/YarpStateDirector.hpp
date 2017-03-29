#ifndef __RD_YARP_STATE_DIRECTOR_HPP__
#define __RD_YARP_STATE_DIRECTOR_HPP__

#include <yarp/os/RateThread.h>

#include "StateDirector.hpp"


namespace rd {


class YarpStateDirector : public StateDirector, private yarp::os::RateThread
{
public:
    YarpStateDirector(State * state);

    /**
     * @brief Function that starts the State execution using YARP
     * It launches a yarp::os::RateThread thread after State setup()
     *
     */
    virtual bool Start();
    virtual bool Stop();

    //! @brief Function called periodically by YARP, that calls the loop() method
    void run();

    //! @brief Period of the calls to the State loop() function
    static const int DEFAULT_RATE_MS;

};

}
#endif // __RD_YARP_STATE_DIRECTOR_HPP__
