// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "RpcResponder.hpp"

#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionWriter.h>
#include <yarp/os/LogStream.h>

#include "LogComponent.hpp"
#include "Vocabs.hpp"

const int rd::RpcResponder::MAX_BELIEF = 600;

bool rd::RpcResponder::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::Bottle in, out;
    in.read(connection);
    out.clear();
    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;

    if (in.get(0).asString() == "help" || in.get(0).asVocab32() == VOCAB_RD_HELP) //-- help
    {
        out.addString("Available commands: help, hit I:id I:damage, login I:id S:name I:team_id, logout I:id.");
    }
    else if (in.get(0).asString() == "hit" || in.get(0).asVocab32() == VOCAB_RD_HIT) //-- hit
    {
        //-- Extract data
        int hitId = in.get(1).asInt32();
        int damage = in.get(2).asInt32();

        //-- Check if the player hit is logged in the game
        players_mutex->lock();
        if ( players->find(hitId) != players->end() )
        {
            int newHealth = players->at( hitId ).getHealth() - damage;
            if (newHealth < 0)
                newHealth = 0;
            if (newHealth > players->at( hitId ).getMaxHealth())
                newHealth = players->at( hitId ).getMaxHealth();
            players->at( hitId ).setHealth( newHealth );

            out.addVocab32(VOCAB_RD_OK);
        }
        else
        {
            yCError(RD_SRV) << "Player with id" << hitId << "not logged in!";
            out.addVocab32(VOCAB_RD_FAIL);
        }
        players_mutex->unlock();
    }
    else if (in.get(0).asString() == "login" || in.get(0).asVocab32() == VOCAB_RD_LOGIN) //-- login
    {
        int loginId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(loginId) == players->end() )  // if not found, we can create
        {
            //-- Player( int id, std::string name, int health, int max_health, int team_id, int score);
            Player rdPlayer( loginId, in.get(2).asString().c_str(),100,100,in.get(3).asInt32(),0);

            (*players)[loginId] = rdPlayer;
            (*players_belief)[loginId] = MAX_BELIEF;

            out.addVocab32(VOCAB_RD_OK);
        }
        else
        {
            yCError(RD_SRV) << "Already logged, id:" << loginId;
            out.addVocab32(VOCAB_RD_FAIL);
        }
        players_mutex->unlock();
    }
    else if (in.get(0).asString() == "logout" || in.get(0).asVocab32() == VOCAB_RD_LOGOUT) //-- logout
    {
        int logoutId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(logoutId) == players->end() )
        {
            yCError(RD_SRV) << "Not logged to logout, id:" << logoutId;
            out.addVocab32(VOCAB_RD_FAIL);
        }
        else
        {
            players->erase(logoutId);
            players_belief->erase(logoutId);
            out.addVocab32(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else if (in.get(0).asString() == "respawn" || in.get(0).asVocab32() == VOCAB_RD_RESPAWN) //-- respawn
    {
        int respawnId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(respawnId) == players->end() )
        {
            yCError(RD_SRV) << "Could not respawn, player not logged in, id:" << respawnId;
            out.addVocab32(VOCAB_RD_FAIL);
        } else {
            int newHealth = players->at(respawnId).getMaxHealth();
            players->at(respawnId).setHealth(newHealth);
            players_belief->at(respawnId) = MAX_BELIEF;
            out.addVocab32(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else if (in.get(0).asString() == "keepAlive" || in.get(0).asVocab32() == VOCAB_RD_KEEPALIVE) //-- keepalive
    {
        int keepAliveId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(keepAliveId) == players->end() )
        {
            yCError(RD_SRV) << "Could not keep alive, player not logged in, id:" << keepAliveId;
            out.addVocab32(VOCAB_RD_FAIL);
        }
        else
        {
            players_belief->at(keepAliveId) = MAX_BELIEF;
            out.addVocab32(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else
    {
        yCError(RD_SRV) << "Unkown command:" << in.toString();
        out.addVocab32(VOCAB_RD_FAIL);
    }

    return out.write(*returnToSender);
}

void rd::RpcResponder::setPlayers(std::map<int,Player> *value, std::map<int, int> *players_belief, std::mutex *players_mutex)
{
    players = value;
    this->players_belief = players_belief;
    this->players_mutex = players_mutex;
}
