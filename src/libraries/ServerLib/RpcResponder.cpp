// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include <yarp/os/Bottle.h>
#include <yarp/os/ConnectionWriter.h>

#include "RpcResponder.hpp"
#include "Macros.hpp"
#include "Vocabs.hpp"

const int rd::RpcResponder::MAX_BELIEF = 600;

bool rd::RpcResponder::read(yarp::os::ConnectionReader& connection)
{
    yarp::os::Bottle in, out;
    in.read(connection);
    //RD_INFO("Got %s\n", in.toString().c_str());
    out.clear();
    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();
    if (returnToSender==NULL) return false;

    if ((in.get(0).asString() == "help")||(in.get(0).asVocab() == VOCAB_RD_HELP))  //-- help
    {
        out.addString("Available commands: help, hit I:id I:damage, login I:id S:name I:team_id, logout I:id.");
    }
    else if((in.get(0).asString() == "hit")||(in.get(0).asVocab() == VOCAB_RD_HIT))  //-- hit
    {
        //-- Extract data
        int hitId = in.get(1).asInt();
        int damage = in.get(2).asInt();

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

            out.addVocab(VOCAB_RD_OK);
        }
        else
        {
            RD_ERROR("Player with id: %d not logged in!\n",hitId);
            out.addVocab(VOCAB_RD_FAIL);
        }
        players_mutex->unlock();
    }
    else if((in.get(0).asString() == "login")||(in.get(0).asVocab() == VOCAB_RD_LOGIN))  //-- login
    {
        int loginId = in.get(1).asInt();

        players_mutex->lock();
        if ( players->find(loginId) == players->end() )  // if not found, we can create
        {
            //-- Player( int id, std::string name, int health, int max_health, int team_id, int score);
            Player rdPlayer( loginId, in.get(2).asString().c_str(),100,100,in.get(3).asInt(),0);

            players->operator [](loginId) = rdPlayer;
            players_belief->operator [](loginId) = MAX_BELIEF;

            out.addVocab(VOCAB_RD_OK);
        }
        else
        {
            RD_ERROR("Already logged, id: %d.\n",loginId);
            out.addVocab(VOCAB_RD_FAIL);
        }
        players_mutex->unlock();
    }
    else if ((in.get(0).asString() == "logout")||(in.get(0).asVocab() == VOCAB_RD_LOGOUT)) //-- logout
    {
        int logoutId = in.get(1).asInt();

        players_mutex->lock();
        if ( players->find(logoutId) == players->end() )
        {
            RD_ERROR("Not logged to logout, id: %d.\n",logoutId);
            out.addVocab(VOCAB_RD_FAIL);
        }
        else
        {
            players->erase(logoutId);
            players_belief->erase(logoutId);
            out.addVocab(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else if ((in.get(0).asString() == "respawn")||(in.get(0).asVocab() == VOCAB_RD_RESPAWN)) //-- respawn
    {
        int respawnId = in.get(1).asInt();

        players_mutex->lock();
        if ( players->find(respawnId) == players->end() )
        {
            RD_ERROR("Could not respawn, player not logged in, id: %d.\n",respawnId);
            out.addVocab(VOCAB_RD_FAIL);
        } else {
            int newHealth = players->at(respawnId).getMaxHealth();
            players->at(respawnId).setHealth(newHealth);
            players_belief->at(respawnId) = MAX_BELIEF;
            out.addVocab(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else if ((in.get(0).asString() == "keepAlive")||(in.get(0).asVocab() == VOCAB_RD_KEEPALIVE)) //-- keepalive
    {
        int keepAliveId = in.get(1).asInt();

        players_mutex->lock();
        if ( players->find(keepAliveId) == players->end() )
        {
            RD_ERROR("Could not keep alive, player not logged in, id: %d.\n",keepAliveId);
            out.addVocab(VOCAB_RD_FAIL);
        }
        else
        {
            players_belief->at(keepAliveId) = MAX_BELIEF;
            out.addVocab(VOCAB_RD_OK);
        }
        players_mutex->unlock();
    }
    else
    {
        RD_ERROR("Unkown command: %s.\n", in.toString().c_str());
        out.addVocab(VOCAB_RD_FAIL);
    }
    return out.write(*returnToSender);

}

void rd::RpcResponder::setPlayers(std::map<int,Player> *value, std::map<int, int> *players_belief, yarp::os::Mutex *players_mutex)
{
    players = value;
    this->players_belief = players_belief;
    this->players_mutex = players_mutex;
}


