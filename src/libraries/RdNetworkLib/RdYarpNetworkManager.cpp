#include "RdYarpNetworkManager.hpp"


//-- Initialize static members
rd::RdYarpNetworkManager * rd::RdYarpNetworkManager::uniqueInstance = NULL;
const std::string rd::RdYarpNetworkManager::id = "YARP";

bool rd::RdYarpNetworkManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new RdYarpNetworkManager();
    }

    return Register( uniqueInstance, id);
}

rd::RdYarpNetworkManager::RdYarpNetworkManager()
{
    started = false;
}

rd::RdYarpNetworkManager::~RdYarpNetworkManager()
{
    uniqueInstance = NULL;
}

bool rd::RdYarpNetworkManager::start()
{
    if (player.getId() == -1)
    {
        RD_ERROR("NetworkManager not initialized, player id not set\n");
        return false;
    }

    if (started)
    {
        RD_ERROR("NetworkManager already started\n");
        return false;
    }

    yarp::os::NetworkBase::initMinimum();

    if ( ! yarp::os::NetworkBase::checkNetwork() )
    {
        RD_ERROR("Found no yarp network to connect to rdServer (try running 'yarpserver &'). Bye!\n");
        return false;
    }

    //-- Open the rcpClient port with this player's id
    std::ostringstream rpc_str;
    rpc_str << "/";
    rpc_str << player.getId();
    rpc_str << "/rdServer/rpc:o";
    if( ! rpcClient.open( rpc_str.str() ) )
    {
        RD_ERROR("Could not open '%s'. Bye!\n",rpc_str.str().c_str());
        return false;
    }

    //-- Open the callback port with this player's id
    std::ostringstream callback_str;
    callback_str << "/";
    callback_str << player.getId();
    callback_str << "/rdServer/command:i";
    if( ! callbackPort.open( callback_str.str() ) )
    {
        RD_ERROR("Could not open '%s'. Bye!\n",callback_str.str().c_str());
        return false;
    }

    //-- Try to rpc connect to the server until timeout
    int tries = 0;
    while(tries++ < 10)
    {
        if(rpcClient.getOutputCount() > 0)
            break;
        RD_INFO("Waiting for rpc to be connected to rdServer (launch 'rdServer' if not already launched). Attempt: %d\n",tries);
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( rpc_str.str() , "/rdServer" );
    }
    if (tries == 11)  //-- 11 allows  10 attempts
    {
        RD_ERROR("Timeout for rpc to be connected to rdServer (launch 'rdServer' if not already launched).\n");
        return false;
    }
    RD_SUCCESS("Rpc connected to Server (outgoing)!\n")

    //-- Try to connect to the broadcast server until timeout
    if ( !yarp::os::Network::connect( "/rdBroadcast", callback_str.str() ))
    {
        RD_ERROR("Error connecting to server (incoming broadcast).\n");
        return false;
    }

    RD_SUCCESS("Connected to Server (incoming broadcast)!\n")

    callbackPort.useCallback(*this);

    started = true;
    return true;
}


void rd::RdYarpNetworkManager::onRead(yarp::os::Bottle &b)
{
    //RD_INFO("Got %s\n", b.toString().c_str());
    if ((b.get(0).asString() == "players")||(b.get(0).asVocab() == VOCAB_RD_PLAYERS)) {  // players //
        //RD_INFO("Number of players: %d\n",b.size()-1);  // -1 because of vocab.
        std::vector< RdPlayer > players;
        for(size_t i=1;i<b.size();i++)
        {

            RdPlayer rdPlayer(b.get(i).asList()->get(0).asInt(),
                              b.get(i).asList()->get(1).asString().c_str(),
                              b.get(i).asList()->get(2).asInt(),
                              b.get(i).asList()->get(3).asInt(),
                              b.get(i).asList()->get(4).asInt(),
                              b.get(i).asList()->get(5).asInt()
                              );
             players.push_back(rdPlayer);
        }

        //-- Notify listeners
        for(int i = 0; i < listeners.size(); i++)
            listeners[i]->onDataArrived(players);
    }
    else
    {
        RD_ERROR("What?\n");
    }

}

bool rd::RdYarpNetworkManager::stop()
{
    if (!started)
    {
        RD_ERROR("Already stopped\n");
        return false;
    }

     rpcClient.close();

     callbackPort.disableCallback();
     callbackPort.interrupt();
     callbackPort.close();

     yarp::os::NetworkBase::finiMinimum();

     started = false;
     return true;
}

bool rd::RdYarpNetworkManager::isStopped()
{
    return !started;
}

bool rd::RdYarpNetworkManager::configure(std::string parameter, RdPlayer value)
{
    if (parameter.compare("player") == 0)
    {
        player = value;
        return true;
    }

    return RdNetworkManager::configure(parameter, value);
}

bool rd::RdYarpNetworkManager::sendPlayerHit(rd::RdPlayer player, int damage)
{
    if (!started)
    {
        RD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    //-- Send a message to the server with the player Id and the damage done:
    yarp::os::Bottle msg_player_hit, response;
    msg_player_hit.addVocab(VOCAB_RD_HIT);
    msg_player_hit.addInt(player.getId());
    msg_player_hit.addInt(damage);
    rpcClient.write(msg_player_hit,response);
    RD_INFO("rdServer response from hit: %s\n",response.toString().c_str());

    //-- Check response
    if ( strcmp( response.toString().c_str(), "[ok]") == 0)
        return true;
    else
        return false;
}

bool rd::RdYarpNetworkManager::login()
{
    if (!started)
    {
        RD_WARNING("NetworkManager has not been started\n");
        if(!start())
        {
            RD_ERROR("RdNetworkManager could not be started for player %d\n", player.getId() );
            return false;
        }
    }

    //-- Start network system
    std::stringstream ss;
    ss << player.getId();

    //-- Send login message
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGIN);
    msgRdPlayer.addInt(player.getId());
    msgRdPlayer.addString(player.getName().c_str());
    msgRdPlayer.addInt(player.getTeamId());
    rpcClient.write(msgRdPlayer,res);
    RD_INFO("rdServer response from login: %s\n",res.toString().c_str());

    //-- Check response
    if ( strcmp( res.toString().c_str(), "[ok]") == 0)
        return true;
    else
        return false;
}

bool rd::RdYarpNetworkManager::logout()
{
    if (!started)
    {
        RD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    RD_INFO("Logout...\n");
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGOUT);
    msgRdPlayer.addInt(player.getId());
    rpcClient.write(msgRdPlayer,res);
    RD_INFO("rdServer response from logout: %s\n",res.toString().c_str());

    //-- Check response
    if ( strcmp( res.toString().c_str(), "[ok]") == 0)
    {
        RD_SUCCESS("Logout ok\n");
        return true;
    }
    else
    {
        RD_ERROR("Logout failed\n");
        return false;
    }
}
