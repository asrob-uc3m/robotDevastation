// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpNetworkManager.hpp"

#include <sstream>
#include <cstring> // strcmp()

#include <yarp/os/Network.h>

#include <ColorDebug.hpp>

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
        CD_ERROR("NetworkManager not initialized, player id not set\n");
        return false;
    }

    if (started)
    {
        CD_ERROR("NetworkManager already started\n");
        return false;
    }

    yarp::os::Network::initMinimum();

    if ( ! yarp::os::Network::checkNetwork() )
    {
        CD_INFO_NO_HEADER("Checking for yarp network... ");
        CD_ERROR_NO_HEADER("[fail]\n");
        CD_INFO_NO_HEADER("Found no yarp network to connect to rdServer (try running \"yarpserver &\"), bye!\n");
        return false;
    }

    //-- Open the rpcClient port with this player's id
    std::ostringstream rpc_str;
    rpc_str << "/robotDevastation/";
    rpc_str << player.getId();
    rpc_str << "/rdServer/rpc:c";
    if( ! rpcClient.open( rpc_str.str() ) )
    {
        CD_ERROR("Could not open '%s'. Bye!\n",rpc_str.str().c_str());
        return false;
    }

    //-- Open the callback port with this player's id
    std::ostringstream callback_str;
    callback_str << "/robotDevastation/";
    callback_str << player.getId();
    callback_str << "/rdServer/info:i";
    if( ! callbackPort.open( callback_str.str() ) )
    {
        CD_ERROR("Could not open '%s'. Bye!\n",callback_str.str().c_str());
        return false;
    }

    //-- Connect robotDevastation RpcClient to rdServer RpcServer
    std::string rdServerRpcS("/rdServer/rpc:s");
    if( ! yarp::os::Network::connect( rpc_str.str() , rdServerRpcS ) )
    {
        CD_INFO_NO_HEADER("Checking for rdServer ports... ");
        CD_ERROR_NO_HEADER("[fail]\n");
        CD_INFO_NO_HEADER("Could not connect to rdServer '%s' port (try running \"rdServer &\"), bye!\n",rdServerRpcS.c_str());
        return false;
    }

    //-- Connect from rdServer info to robotDevastation callbackPort
    std::string rdServerInfoO("/rdServer/info:o");
    if ( ! yarp::os::Network::connect( rdServerInfoO, callback_str.str() ))
    {
        CD_INFO_NO_HEADER("Checking for rdServer ports... ");
        CD_ERROR_NO_HEADER("[fail]\n");
        CD_INFO_NO_HEADER("Could not connect from rdServer '%s' port (try running \"rdServer &\"), bye!\n",rdServerInfoO.c_str());
        return false;
    }
    CD_INFO_NO_HEADER("Checking for rdServer ports... ");
    CD_SUCCESS_NO_HEADER("[ok]\n");

    callbackPort.useCallback(*this);

    RateThread::start();

    started = true;
    return true;
}


void rd::YarpNetworkManager::onRead(yarp::os::Bottle &b)
{
    //CD_INFO("Got %s\n", b.toString().c_str());
    if ((b.get(0).asString() == "players")||(b.get(0).asVocab() == VOCAB_RD_PLAYERS)) {  // players //
        //CD_INFO("Number of players: %d\n",b.size()-1);  // -1 because of vocab.
        std::vector< Player > players;
        for (int i = 1; i < b.size(); i++)
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
        for (std::vector<NetworkEventListener *>::iterator it = listeners.begin(); it != listeners.end(); ++it)
        {
            (*it)->onDataArrived(players);
        }
    }
    else
    {
        CD_ERROR("What?\n");
    }

}

bool rd::YarpNetworkManager::stop()
{
    if (!started)
    {
        CD_ERROR("Already stopped\n");
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

bool rd::YarpNetworkManager::configure(const std::string & parameter, const Player & value)
{
    if (parameter.compare("player") == 0)
    {
        player = value;
        return true;
    }

    return NetworkManager::configure(parameter, value);
}

bool rd::YarpNetworkManager::sendPlayerHit(const Player & player, int damage)
{
    if (!started)
    {
        CD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    //-- Send a message to the server with the player Id and the damage done:
    yarp::os::Bottle msg_player_hit, response;
    msg_player_hit.addVocab(VOCAB_RD_HIT);
    msg_player_hit.addInt(player.getId());
    msg_player_hit.addInt(damage);
    rpcClient.write(msg_player_hit,response);
    CD_INFO("rdServer response from hit: %s\n",response.toString().c_str());

    //-- Check response
    if (std::strcmp(response.toString().c_str(), "[ok]") == 0)
        return true;
    else
        return false;
}

bool rd::YarpNetworkManager::login()
{
    if ( ! started )
    {
        CD_WARNING("NetworkManager has not been started\n");
        if( ! start() )
        {
            CD_ERROR("NetworkManager could not be started for player %d\n", player.getId() );
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
    CD_INFO("rdServer response from login: %s\n",res.toString().c_str());

    //-- Check response
    if (std::strcmp(res.toString().c_str(), "[ok]") == 0)
        return true;
    else
        return false;
}

bool rd::YarpNetworkManager::logout()
{
    if (!started)
    {
        CD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    CD_INFO("Logout...\n");
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGOUT);
    msgRdPlayer.addInt(player.getId());
    rpcClient.write(msgRdPlayer,res);
    CD_INFO("rdServer response from logout: %s\n",res.toString().c_str());

    //-- Check response
    if (std::strcmp(res.toString().c_str(), "[ok]") == 0)
    {
        CD_SUCCESS("Logout ok\n");
        return true;
    }
    else
    {
        CD_ERROR("Logout failed\n");
        return false;
    }
}

bool rd::YarpNetworkManager::keepAlive()
{
    if (!started)
    {
        CD_ERROR("NetworkManager has not been started\n");
        return false;
    }

    CD_INFO("Keep alive...\n");
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_KEEPALIVE);
    msgRdPlayer.addInt(player.getId());
    rpcClient.write(msgRdPlayer,res);

    //-- Check response
    if (std::strcmp(res.toString().c_str(), "[ok]") == 0)
    {
        return true;
    }
    else
    {
        CD_ERROR("Keep alive failed\n");
        return false;
    }
}
