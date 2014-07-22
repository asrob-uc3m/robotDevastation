// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdRpcResponder.hpp"

bool rd::RdRpcResponder::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::Bottle in, out;
    in.read(connection);
    RD_INFO("Got %s\n", in.toString().c_str());
    out.clear();
    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;

    if ((in.get(0).asString() == "login")||(in.get(0).asVocab() == VOCAB_RD_LOGIN)) {  // login //

        RdPlayer rdPlayer(in.get(1).asInt(),in.get(2).asString(),100,in.get(3).asInt());
        players.push_back(rdPlayer);

        out.addVocab(VOCAB_RD_OK);
    }
    else
    {
        out.addVocab(VOCAB_RD_FAIL);
    }
    return out.write(*returnToSender);

}
