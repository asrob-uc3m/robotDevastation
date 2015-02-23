#ifndef YARPSTATEDIRECTOR_HPP
#define YARPSTATEDIRECTOR_HPP

#include "StateDirector.hpp"

namespace rd {


class YarpStateDirector : public StateDirector
{
public:
    YarpStateDirector(State * state) : StateDirector(state) {}


    virtual bool start() {return false;}
    virtual bool stop() {return false;}

    virtual bool addTransition( StateDirector * nextState, int condition) {return false;}

};

}
#endif // YARPSTATEDIRECTOR_HPP
