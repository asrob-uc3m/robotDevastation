// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "Server.hpp"

#include <cstdio>

#include <yarp/os/Bottle.h>

#include "Macros.hpp"
#include "Vocabs.hpp"

bool rd::Server::configure(yarp::os::ResourceFinder &rf)
{
    quiet = false;
    if (rf.check("quiet"))
    {
        RD_INFO("stdout feedback disabled\n");
        quiet = true;
    }

    rdRpcResponder.setPlayers(&players, &players_belief, &players_mutex);
    rdBroadcast.open("/rdServer/info:o");
    rpcServer.open("/rdServer/rpc:s");
    rpcServer.setReader(rdRpcResponder);
    return true;
}

rd::Server::~Server()
{

}

double rd::Server::getPeriod()
{
    return 0.1;  // Fixed, in seconds, the slow thread that calls updateModule below
}

bool rd::Server::updateModule()
{
    //-- Broadcast player info
    players_mutex.lock();
    yarp::os::Bottle msgBroadcast;
    msgBroadcast.addVocab(VOCAB_RD_PLAYERS);
    quiet || std::printf("=======rdServer=======\n");
    quiet || std::printf("Number of players: %zd\n",players.size());

    typedef std::map<int, Player>::iterator it_type;
    for(it_type iterator = players.begin(); iterator != players.end(); iterator++)
    {
        quiet || std::printf("----------------------\n%s\n", (iterator->second).str().c_str());
        quiet || std::printf("Belief: %d\n", players_belief[iterator->first]);
        yarp::os::Bottle msgPlayer;
        msgPlayer.addInt( (iterator->second).getId() );
        msgPlayer.addString( (iterator->second).getName().c_str() );
        msgPlayer.addInt( (iterator->second).getHealth() );
        msgPlayer.addInt( (iterator->second).getMaxHealth() );
        msgPlayer.addInt( (iterator->second).getTeamId() );
        msgPlayer.addInt( (iterator->second).getScore() );
        msgBroadcast.addList() = msgPlayer;
    }

    //std::printf("======================\n");
    rdBroadcast.write(msgBroadcast);
    players_mutex.unlock();

    //-- Decrease players belief
    players_mutex.lock();
    for(it_type it = players.begin(); it != players.end(); )
    {
        //-- Erasing current map element only works in a loop if keeping track
        //-- of the next element by hand: http://stackoverflow.com/questions/5671733/c-map-iteration-with-deletion#5673209
        it_type next = it;
        ++next;

        //-- Decrease belief and erase if required
        int id = it->first;
        players_belief[id] -= 1;
        if (players_belief[id]<=0)
        {
            //-- Remove player from player table
            players.erase(id);
            players_belief.erase(id);
        }

        it = next;
    }
    players_mutex.unlock();

    return true;
}

bool rd::Server::interruptModule() {
    std::printf("Server closing...\n");
    rdBroadcast.interrupt();
    rpcServer.interrupt();
    rdBroadcast.close();
    rpcServer.close();
    return true;
}
