#ifndef YARPSTATEDIRECTOR_HPP
#define YARPSTATEDIRECTOR_HPP

#include <yarp/os/RateThread.h>

#include "StateDirector.hpp"


namespace rd {


class YarpStateDirector : public StateDirector, private yarp::os::RateThread
{
public:
    YarpStateDirector(State * state);


    virtual bool Start();
    virtual bool Stop();

    void run();  // The periodical function

    static const int DEFAULT_RATE_MS;
};

}
#endif // YARPSTATEDIRECTOR_HPP
