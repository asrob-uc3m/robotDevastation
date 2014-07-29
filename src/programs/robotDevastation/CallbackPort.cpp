// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CallbackPort.hpp"

void rd::CallbackPort::onRead(yarp::os::Bottle& b)
{
    RD_INFO("Got %s\n", b.toString().c_str());

}
