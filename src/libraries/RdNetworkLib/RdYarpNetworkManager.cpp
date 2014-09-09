#include "RdYarpNetworkManager.hpp"

//-- This is very important:
rd::RdNetworkManager * rd::RdYarpNetworkManager::networkManagerInstance = NULL;

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

    delete networkManagerInstance;
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

        for(int i = 0; i < listeners.size(); i++)
            listeners[i]->onDataArrived(players);
    }
    else
    {
        RD_ERROR("What?\n");
    }

}

rd::RdYarpNetworkManager::~RdYarpNetworkManager()
{

}
