// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MOCK_NETWORK_MANAGER_HPP__
#define __RD_MOCK_NETWORK_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>

#include "NetworkManager.hpp"
#include "NetworkEventListener.hpp"
#include "Player.hpp"

namespace rd{

/**
 * @ingroup NetworkLib
 *
 * @brief Allow for low-level access of members in tests / manipulating internal data
 *
 * MockNetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the MockNetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the MockNetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived).
 *
 */

class MockNetworkManager : public NetworkManager
{
    public:
        //-- Creation and configuration
        //--------------------------------------------------------------------------------------------
        /**
         * @brief Register this manager in the NetworkManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance).
         */
        static bool RegisterManager();

        virtual ~MockNetworkManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        virtual bool start();
        virtual bool stop();

        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const Player & value);

        //-- Server API
        //--------------------------------------------------------------------------------------------
        virtual bool sendPlayerHit(const Player & player, int damage);
        virtual bool login();
        virtual bool logout();
        virtual bool keepAlive();

        //-- Mock object API
        //--------------------------------------------------------------------------------------------
        bool isLoggedIn() const;
        bool isStopped() const;

        bool setPlayerData(const std::vector<Player> & game_players);
        std::vector<Player> getPlayerData() const;
        bool sendPlayerData();
        bool setLoggedIn(bool logged_in);

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the RegisterManager() method.
         */
        MockNetworkManager();

        MockNetworkManager(const MockNetworkManager &);
        MockNetworkManager & operator=(const MockNetworkManager &);

        //! @brief Reference to this manager (unique instance)
        static MockNetworkManager * uniqueInstance;


        //-- Required stuff
        bool logged_in;
        bool stopped;
        Player player;
        std::vector<Player> game_players;
        std::map <int, Player> players_dic;

};
}

#endif //-- __RD_MOCK_NETWORK_MANAGER_HPP__
