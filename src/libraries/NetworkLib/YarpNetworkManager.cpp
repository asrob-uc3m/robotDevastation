// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#include "YarpNetworkManager.hpp"

#include <sstream>

#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>

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

rd::YarpNetworkManager::YarpNetworkManager() : PeriodicThread(KEEPALIVE_RATE_MS * 0.001)
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
        yError() << "NetworkManager not initialized, player id not set";
        return false;
    }

    if (started)
    {
        yError() << "NetworkManager already started";
        return false;
    }

    yarp::os::Network::initMinimum();

    if ( ! yarp::os::Network::checkNetwork() )
    {
        yError() << "Found no yarp network to connect to rdServer (try running \"yarpserver &\"), bye!";
        return false;
    }

    //-- Open the rpcClient port with this player's id
    std::ostringstream rpc_str;
    rpc_str << "/robotDevastation/";
    rpc_str << player.getId();
    rpc_str << "/rdServer/rpc:c";
    if( ! rpcClient.open( rpc_str.str() ) )
    {
        yError() << "Could not open" << rpc_str.str();
        return false;
    }

    //-- Open the callback port with this player's id
    std::ostringstream callback_str;
    callback_str << "/robotDevastation/";
    callback_str << player.getId();
    callback_str << "/rdServer/info:i";
    if( ! callbackPort.open( callback_str.str() ) )
    {
        yError() << "Could not open" << callback_str.str();
        return false;
    }

    //-- Connect robotDevastation RpcClient to rdServer RpcServer
    std::string rdServerRpcS("/rdServer/rpc:s");
    if( ! yarp::os::Network::connect( rpc_str.str() , rdServerRpcS ) )
    {
        yError() << "Could not connect to rdServer" << rdServerRpcS << "port (try running \"rdServer &\")";
        return false;
    }

    //-- Connect from rdServer info to robotDevastation callbackPort
    std::string rdServerInfoO("/rdServer/info:o");
    if ( ! yarp::os::Network::connect( rdServerInfoO, callback_str.str() ))
    {
        yError() << "Could not connect from rdServer" << rdServerInfoO << "port (try running \"rdServer &\")";
        return false;
    }

    callbackPort.useCallback(*this);

    PeriodicThread::start();

    started = true;
    return true;
}


void rd::YarpNetworkManager::onRead(yarp::os::Bottle &b)
{
    if ((b.get(0).asString() == "players")||(b.get(0).asVocab() == VOCAB_RD_PLAYERS)) {  // players //
        std::vector< Player > players;
        for (int i = 1; i < b.size(); i++)
        {
            Player rdPlayer(b.get(i).asList()->get(0).asInt32(),
                              b.get(i).asList()->get(1).asString().c_str(),
                              b.get(i).asList()->get(2).asInt32(),
                              b.get(i).asList()->get(3).asInt32(),
                              b.get(i).asList()->get(4).asInt32(),
                              b.get(i).asList()->get(5).asInt32()
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
        yError() << "What?";
    }

}

bool rd::YarpNetworkManager::stop()
{
    if (!started)
    {
        yError() << "Already stopped";
        return false;
    }

    PeriodicThread::askToStop();

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
    return ! started;
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
    if ( ! started )
    {
        yError() << "NetworkManager has not been started";
        return false;
    }

    //-- Send a message to the server with the player Id and the damage done:
    yarp::os::Bottle msg_player_hit, response;
    msg_player_hit.addVocab(VOCAB_RD_HIT);
    msg_player_hit.addInt32(player.getId());
    msg_player_hit.addInt32(damage);
    rpcClient.write(msg_player_hit,response);
    yInfo() << "rdServer response from hit:" << response.toString();

    //-- Check response
    if (response.toString() != "[ok]")
        return false;

    return true;
}

bool rd::YarpNetworkManager::login()
{
    if ( ! started )
    {
        yWarning() << "NetworkManager has not been started";
        if( ! start() )
        {
            yError() << "NetworkManager could not be started for player" << player.getId();
            return false;
        }
    }

    //-- Start network system
    std::stringstream ss;
    ss << player.getId();

    //-- Send login message
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGIN);
    msgRdPlayer.addInt32(player.getId());
    msgRdPlayer.addString(player.getName().c_str());
    msgRdPlayer.addInt32(player.getTeamId());
    rpcClient.write(msgRdPlayer,res);
    yInfo() << "rdServer response from login:" << res.toString();

    //-- Check response
    if (res.toString() != "[ok]")
        return false;

    return true;
}

bool rd::YarpNetworkManager::logout()
{
    if (!started)
    {
        yError() << "NetworkManager has not been started";
        return false;
    }

    yInfo() << "Logout...";
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_LOGOUT);
    msgRdPlayer.addInt32(player.getId());
    rpcClient.write(msgRdPlayer,res);
    yInfo() << "rdServer response from logout:" << res.toString();

    //-- Check response
    if (res.toString() != "[ok]")
    {
        yError() << "Logout failed";
        return false;
    }

    return true;
}

bool rd::YarpNetworkManager::keepAlive()
{
    if (!started)
    {
        yError() << "NetworkManager has not been started";
        return false;
    }

    yInfo() << "Keep alive...";
    yarp::os::Bottle msgRdPlayer,res;
    msgRdPlayer.addVocab(VOCAB_RD_KEEPALIVE);
    msgRdPlayer.addInt32(player.getId());
    rpcClient.write(msgRdPlayer,res);

    //-- Check response
    if (res.toString() != "[ok]")
    {
        yError() << "Keep alive failed";
        return false;
    }

    return true;
}
