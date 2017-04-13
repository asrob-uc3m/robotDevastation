#include "YarpNetworkManager.hpp"

#include <string.h> // strcmp()
#include <sstream>

#include <yarp/os/Network.h>

#include "Macros.hpp"
#include "Vocabs.hpp"

//-- Initialize static members
rd::YarpNetworkManager * rd::YarpNetworkManager::uniqueInstance = NULL;
const std::string rd::YarpNetworkManager::id = "YARP";
const int rd::YarpNetworkManager::KEEPALIVE_RATE_MS = 1000;

bool rd::YarpNetworkManager::RegisterManager()
{
    if (uniqueInstance == NULL)
    {
        uniqueInstance = new YarpNetworkManager();
    }

    return Register( uniqueInstance, id);
}

rd::YarpNetworkManager::YarpNetworkManager() : RateThread(KEEPALIVE_RATE_MS)
{
    started = false;
}

void rd::YarpNetworkManager::run()
{
    keepAlive();
}

rd::YarpNetworkManager::~YarpNetworkManager()
{
    uniqueInstance = NULL;
}

bool rd::YarpNetworkManager::start()
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
    rpc_str << "/robotDevastation/";
    rpc_str << player.getId();
    rpc_str << "/rdServer/rpc:c";
    if( ! rpcClient.open( rpc_str.str() ) )
    {
        RD_ERROR("Could not open '%s'. Bye!\n",rpc_str.str().c_str());
        return false;
    }

    //-- Open the callback port with this player's id
    std::ostringstream callback_str;
    callback_str << "/robotDevastation/";
    callback_str << player.getId();
    callback_str << "/rdServer/info:i";
    if( ! callbackPort.open( callback_str.str() ) )
    {
        RD_ERROR("Could not open '%s'. Bye!\n",callback_str.str().c_str());
        return false;
    }

    //-- Connect robotDevastation RpcClient to rdServer RpcServer
    if( ! yarp::os::Network::connect( rpc_str.str() , "/rdServer/rpc:s" ) )
    {
        RD_ERROR("Could not connect robotDevastation RpcClient to rdServer RpcServer (launch 'rdServer' if not already launched).\n");
        return false;
    }
    RD_SUCCESS("Connected robotDevastation RpcClient to rdServer RpcServer!\n");

    //-- Connect from rdServer info to robotDevastation callbackPort
    if ( !yarp::os::Network::connect( "/rdServer/info:o", callback_str.str() ))
    {
        RD_ERROR("Could not connect from rdServer info to robotDevastation callbackPort (launch 'rdServer' if not already launched).\n");
        return false;
    }
    RD_SUCCESS("Connected from rdServer info to robotDevastation callbackPort!\n");

    callbackPort.useCallback(*this);

    RateThread::start();

    started = true;
    return true;
}


void rd::YarpNetworkManager::onRead(yarp::os::Bottle &b)
{
    //RD_INFO("Got %s\n", b.toString().c_str());
    if ((b.get(0).asString() == "players")||(b.get(0).asVocab() == VOCAB_RD_PLAYERS)) {  // players //
        //RD_INFO("Number of players: %d\n",b.size()-1);  // -1 because of vocab.
        std::vector< Player > players;
        for(size_t i=1;i<b.size();i++)
        {

            Player rdPlayer(b.get(i).asList()->get(0).asInt(),
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

bool rd::YarpNetworkManager::stop()
{
    if (!started)
    {
        RD_ERROR("Already stopped\n");
        return false;
    }

    RateThread::askToStop();

    rpcClient.close();

    callbackPort.disableCallback();
    callbackPort.interrupt();
    callbackPort.close();

    yarp::os::NetworkBase::finiMinimum();

    started = false;
    return true;
}

bool rd::YarpNetworkManager::isStopped() const
{
    return !started;
}

bool rd::YarpNetworkManager::configure(const std::string & parameter, Player value)
{
    if (parameter.compare("player") == 0)
    {
        player = value;
        return true;
    }

    return NetworkManager::configure(parameter, value);
}

bool rd::YarpNetworkManager::sendPlayerHit(Player player, int damage)
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

bool rd::YarpNetworkManager::login()
{
    if (!started)
    {
        RD_WARNING("NetworkManager has not been started\n");
        if(!start())
        {
            RD_ERROR("NetworkManager could not be started for player %d\n", player.getId() );
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

bool rd::YarpNetworkManager::logout()
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

bool rd::YarpNetworkManager::keepAlive()
{
    if (!started)
    {
        RD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    RD_INFO("Keep alive...\n");
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_KEEPALIVE);
    msgRdPlayer.addInt(player.getId());
    rpcClient.write(msgRdPlayer,res);

    //-- Check response
    if ( strcmp( res.toString().c_str(), "[ok]") == 0)
    {
        return true;
    }
    else
    {
        RD_ERROR("Keep alive failed\n");
        return false;
    }
}
