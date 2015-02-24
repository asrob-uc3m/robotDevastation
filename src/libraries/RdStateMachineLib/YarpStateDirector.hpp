#ifndef YARPSTATEDIRECTOR_HPP
#define YARPSTATEDIRECTOR_HPP

#include <yarp/os/RateThread.h>

#include "StateDirector.hpp"


namespace rd {


class YarpStateDirector : public StateDirector, private yarp::os::RateThread
{
public:
    YarpStateDirector(State * state);


    virtual bool start();
    virtual bool stop();

    void run();  // The periodical function

    virtual bool addTransition( StateDirector * nextState, int condition);

    static const int DEFAULT_RATE_MS;
};

}
#endif // YARPSTATEDIRECTOR_HPP
