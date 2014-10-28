// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdRpcResponder.hpp"

bool rd::RdRpcResponder::read(yarp::os::ConnectionReader& connection)
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
        if ( players->find(hitId) != players->end() )
        {
            int newHealth = players->at( hitId ).getHealth() - damage;
            if (newHealth < 0)
                newHealth = 0;
            players->at( hitId ).setHealth( newHealth );

            out.addVocab(VOCAB_RD_OK);
        }
        else
        {
            RD_ERROR("Player with id: %d not logged in!\n",hitId);
            out.addVocab(VOCAB_RD_FAIL);
        }
    }
    else if((in.get(0).asString() == "login")||(in.get(0).asVocab() == VOCAB_RD_LOGIN))  //-- login
    {
        int loginId = in.get(1).asInt();

        if ( players->find(loginId) == players->end() )  // if not found, we can create
        {
            //-- RdPlayer( int id, std::string name, int health, int max_health, int team_id, int score);
            RdPlayer rdPlayer( loginId, in.get(2).asString().c_str(),100,100,in.get(3).asInt(),0);

            players->operator[](loginId) = rdPlayer;

            out.addVocab(VOCAB_RD_OK);
        }
        else
        {
            RD_ERROR("Already logged, id: %d.\n",loginId);
            out.addVocab(VOCAB_RD_FAIL);
        }
    }
    else if ((in.get(0).asString() == "logout")||(in.get(0).asVocab() == VOCAB_RD_LOGOUT)) //-- logout
    {
        int logoutId = in.get(1).asInt();

        if ( players->find(logoutId) == players->end() )
        {
            RD_ERROR("Not logged to logout, id: %d.\n",logoutId);
            out.addVocab(VOCAB_RD_FAIL);
        } else {
            players->erase(logoutId);
            out.addVocab(VOCAB_RD_OK);
        }
    }
    else
    {
		RD_ERROR("Unkown command: %s.\n", in.toString().c_str());
        out.addVocab(VOCAB_RD_FAIL);
    }
    return out.write(*returnToSender);

}

void rd::RdRpcResponder::setPlayers(std::map<int,RdPlayer> *value)
{
    players = value;
}


