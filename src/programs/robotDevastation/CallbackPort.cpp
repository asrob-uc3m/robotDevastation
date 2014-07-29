// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "CallbackPort.hpp"

void rd::CallbackPort::onRead(yarp::os::Bottle& b)
{
    //RD_INFO("Got %s\n", b.toString().c_str());
    if ((b.get(0).asString() == "players")||(b.get(0).asVocab() == VOCAB_RD_PLAYERS)) {  // players //
        //RD_INFO("Number of players: %d\n",b.size()-1);  // -1 because of vocab.
        playersSemaphore->wait();
        players->clear();
        for(size_t i=1;i<b.size();i++)
        {

            RdPlayer rdPlayer(b.get(i).asList()->get(0).asInt(),
                              b.get(i).asList()->get(1).asString().c_str(),
                              b.get(i).asList()->get(2).asInt(),
                              b.get(i).asList()->get(3).asInt(),
                              b.get(i).asList()->get(4).asInt(),
                              b.get(i).asList()->get(5).asInt()
                              );
            players->push_back(rdPlayer);
        }
        playersSemaphore->post();
    }
    else
    {
        RD_ERROR("What?\n");
    }


}

void rd::CallbackPort::setPlayers(std::vector<RdPlayer> *value)
{
    players = value;
}

void rd:: CallbackPort::setPlayersSemaphore(yarp::os::Semaphore *value)
{
    playersSemaphore = value;
}

