// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __MOCKUP_NETWORK_MANAGER_HPP__
#define __MOCKUP_NETWORK_MANAGER_HPP__

#include "RdUtils.hpp"
#include "RdNetworkManager.hpp"
#include "RdNetworkEventListener.hpp"
#include "RdPlayer.hpp"

namespace rd{

/**
 * @ingroup RdNetworkLib
 *
 * @brief Allow for low-level access of members in tests / manipulating internal data
 *
 * MockupNetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the MockupNetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the MockupNetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived)
 *
 */

class MockupNetworkManager : RdNetworkManager
{
    public:
        //-- Creation and configuration
        //--------------------------------------------------------------------------------------------
        /**
         * @brief Register this manager in the RdNetworkManager registry so that can be used
         *
         * It ensures that only one manager of this type is created (unique instance)
         */
        static bool RegisterManager();

        ~MockupNetworkManager();

        //! @brief String that identifies this manager
        static const std::string id;

        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        virtual bool start();
        virtual bool stop();

        //! @brief Configures a parameter with a value
        virtual bool configure(std::string parameter, std::string value);

        //-- RdServer API
        //--------------------------------------------------------------------------------------------
        virtual bool sendPlayerHit(RdPlayer player, int damage);
        virtual bool login(RdPlayer player);
        virtual bool logout(RdPlayer player);

        //-- Mockup object API
        //--------------------------------------------------------------------------------------------
        bool isLoggedIn();
        bool isStopped();

        bool setPlayerData(std::vector<RdPlayer> players);
        std::vector<RdPlayer> getPlayerData();
        bool sendPlayerData();
        bool setLoggedIn(bool logged_in);


    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getNetworkManager() method
         */
        MockupNetworkManager();

        //! @brief Reference to this manager (unique instance)
        static MockupNetworkManager * uniqueInstance;

        //-- Required stuff
        bool logged_in;
        bool stopped;
        std::vector<RdPlayer> players;

        std::map <int, RdPlayer> players_dic;

};
}

#endif //-- __RD_NETWORK_MANAGER_HPP__
