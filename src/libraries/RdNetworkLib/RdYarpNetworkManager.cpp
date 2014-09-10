#include "RdYarpNetworkManager.hpp"


rd::RdNetworkManager *rd::RdYarpNetworkManager::getNetworkManager()
{
    if( networkManagerInstance == NULL )
        networkManagerInstance = new RdYarpNetworkManager();

    return networkManagerInstance;
}

bool rd::RdYarpNetworkManager::destroyNetworkManager()
{
    if (networkManagerInstance == NULL)
        return false;

    delete (RdYarpNetworkManager *) networkManagerInstance;
    networkManagerInstance = NULL;

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

bool rd::RdYarpNetworkManager::start(int id)
{
    yarp::os::Network yarp; /// \todo Ask Juan about this

    //-- Open the rcpClient port with this player's id
    std::ostringstream s;
    s << id;
    rpcClient.open(("/rpc/"+s.str()).c_str());
    this->open(("/callback/"+s.str()).c_str());
    this->useCallback();

    //-- Try to connect to the server until timeout
    int tries = 0;
    while(tries++ < 10)
    {
        if(rpcClient.getOutputCount() > 0)
            break;
        RD_INFO("Waiting for rpc to be connected to server...\n");
        yarp::os::Time::delay(0.5);
        yarp::os::Network::connect( ("/rpc/"+s.str()).c_str() , "/rdServer" );
    }
    yarp::os::Network::connect( "/rdBroadcast", ("/callback/"+s.str()).c_str() );

    if (tries == 10)
    {
        RD_ERROR("Timeout!\n");
        return false;
    }
    else
    {
        return true;
    }
}

rd::RdYarpNetworkManager::RdYarpNetworkManager()
{
}

rd::RdYarpNetworkManager::~RdYarpNetworkManager()
{
    rpcClient.close();
    this->interrupt();
    this->close();
}

bool rd::RdYarpNetworkManager::sendPlayerHit(rd::RdPlayer player, int damage)
{
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

bool rd::RdYarpNetworkManager::login(rd::RdPlayer player)
{
    //-- Start network system
    if( !start(player.getId()))
    {
        RD_ERROR("RdNetworkManager could not be started for player %d", player.getId() );
        return false;
    }

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

bool rd::RdYarpNetworkManager::logout(rd::RdPlayer player)
{
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