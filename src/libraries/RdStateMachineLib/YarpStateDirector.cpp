#include "YarpStateDirector.hpp"

const int rd::YarpStateDirector::DEFAULT_RATE_MS = 10;

rd::YarpStateDirector::YarpStateDirector(rd::State *state) : StateDirector(state), RateThread(DEFAULT_RATE_MS)
{

}

bool rd::YarpStateDirector::start()
{
    return yarp::os::RateThread::start();
}

bool rd::YarpStateDirector::stop()
{
    yarp::os::RateThread::askToStop();
    yarp::os::RateThread::stop();
    return true;
}

void rd::YarpStateDirector::run()
{

}

bool rd::YarpStateDirector::addTransition(rd::StateDirector *nextState, int condition)
{
    return false;
}
