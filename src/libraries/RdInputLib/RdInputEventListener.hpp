// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_INPUT_EVENT_LISTENER_HPP__
#define __RD_INPUT_EVENT_LISTENER_HPP__

#include "RdKey.hpp"

namespace rd{

class RdInputEventListener
{
    public:
        virtual bool onKeyDown( RdKey k ) = 0;
        virtual bool onKeyUp( RdKey k ) = 0;

};

}

#endif //-- __RD_INPUT_EVENT_LISTENER_HPP__
