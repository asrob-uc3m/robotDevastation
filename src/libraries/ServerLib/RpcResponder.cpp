// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "RpcResponder.hpp"

#include <yarp/conf/version.h>
#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionWriter.h>
#include <yarp/os/LogStream.h>

#include "Vocabs.hpp"

const int rd::RpcResponder::MAX_BELIEF = 600;

bool rd::RpcResponder::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::Bottle in, out;
    in.read(connection);
    out.clear();
    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;

#if YARP_VERSION_MINOR >= 5
    if (in.get(0).asString() == "help" || in.get(0).asVocab32() == VOCAB_RD_HELP) //-- help
#else
    if (in.get(0).asString() == "help" || in.get(0).asVocab() == VOCAB_RD_HELP) //-- help
#endif
    {
        out.addString("Available commands: help, hit I:id I:damage, login I:id S:name I:team_id, logout I:id.");
    }
#if YARP_VERSION_MINOR >= 5
    else if (in.get(0).asString() == "hit" || in.get(0).asVocab32() == VOCAB_RD_HIT) //-- hit
#else
    else if (in.get(0).asString() == "hit" || in.get(0).asVocab() == VOCAB_RD_HIT) //-- hit
#endif
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

#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_OK);
#else
            out.addVocab(VOCAB_RD_OK);
#endif
        }
        else
        {
            yError() << "Player with id" << hitId << "not logged in!";
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_FAIL);
#else
            out.addVocab(VOCAB_RD_FAIL);
#endif
        }
        players_mutex->unlock();
    }
#if YARP_VERSION_MINOR >= 5
    else if (in.get(0).asString() == "login" || in.get(0).asVocab32() == VOCAB_RD_LOGIN) //-- login
#else
    else if (in.get(0).asString() == "login" || in.get(0).asVocab() == VOCAB_RD_LOGIN) //-- login
#endif
    {
        int loginId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(loginId) == players->end() )  // if not found, we can create
        {
            //-- Player( int id, std::string name, int health, int max_health, int team_id, int score);
            Player rdPlayer( loginId, in.get(2).asString().c_str(),100,100,in.get(3).asInt32(),0);

            (*players)[loginId] = rdPlayer;
            (*players_belief)[loginId] = MAX_BELIEF;

#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_OK);
#else
            out.addVocab(VOCAB_RD_OK);
#endif
        }
        else
        {
            yError() << "Already logged, id:" << loginId;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_FAIL);
#else
            out.addVocab(VOCAB_RD_FAIL);
#endif
        }
        players_mutex->unlock();
    }
#if YARP_VERSION_MINOR >= 5
    else if (in.get(0).asString() == "logout" || in.get(0).asVocab32() == VOCAB_RD_LOGOUT) //-- logout
#else
    else if (in.get(0).asString() == "logout" || in.get(0).asVocab() == VOCAB_RD_LOGOUT) //-- logout
#endif
    {
        int logoutId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(logoutId) == players->end() )
        {
            yError() << "Not logged to logout, id:" << logoutId;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_FAIL);
#else
            out.addVocab(VOCAB_RD_FAIL);
#endif
        }
        else
        {
            players->erase(logoutId);
            players_belief->erase(logoutId);
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_OK);
#else
            out.addVocab(VOCAB_RD_OK);
#endif
        }
        players_mutex->unlock();
    }
#if YARP_VERSION_MINOR >= 5
    else if (in.get(0).asString() == "respawn" || in.get(0).asVocab32() == VOCAB_RD_RESPAWN) //-- respawn
#else
    else if (in.get(0).asString() == "respawn" || in.get(0).asVocab() == VOCAB_RD_RESPAWN) //-- respawn
#endif
    {
        int respawnId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(respawnId) == players->end() )
        {
            yError() << "Could not respawn, player not logged in, id:" << respawnId;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_FAIL);
#else
            out.addVocab(VOCAB_RD_FAIL);
#endif
        } else {
            int newHealth = players->at(respawnId).getMaxHealth();
            players->at(respawnId).setHealth(newHealth);
            players_belief->at(respawnId) = MAX_BELIEF;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_OK);
#else
            out.addVocab(VOCAB_RD_OK);
#endif
        }
        players_mutex->unlock();
    }
#if YARP_VERSION_MINOR >= 5
    else if (in.get(0).asString() == "keepAlive" || in.get(0).asVocab32() == VOCAB_RD_KEEPALIVE) //-- keepalive
#else
    else if (in.get(0).asString() == "keepAlive" || in.get(0).asVocab() == VOCAB_RD_KEEPALIVE) //-- keepalive
#endif
    {
        int keepAliveId = in.get(1).asInt32();

        players_mutex->lock();
        if ( players->find(keepAliveId) == players->end() )
        {
            yError() << "Could not keep alive, player not logged in, id:" << keepAliveId;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_FAIL);
#else
            out.addVocab(VOCAB_RD_FAIL);
#endif
        }
        else
        {
            players_belief->at(keepAliveId) = MAX_BELIEF;
#if YARP_VERSION_MINOR >= 5
            out.addVocab32(VOCAB_RD_OK);
#else
            out.addVocab(VOCAB_RD_OK);
#endif
        }
        players_mutex->unlock();
    }
    else
    {
        yError() << "Unkown command:" << in.toString();
#if YARP_VERSION_MINOR >= 5
        out.addVocab32(VOCAB_RD_FAIL);
#else
        out.addVocab(VOCAB_RD_FAIL);
#endif
    }

    return out.write(*returnToSender);
}

void rd::RpcResponder::setPlayers(std::map<int,Player> *value, std::map<int, int> *players_belief, std::mutex *players_mutex)
{
    players = value;
    this->players_belief = players_belief;
    this->players_mutex = players_mutex;
}
