// Authors: see AUTHORS.md at project root.
// CopyPolicy: released under the terms of the LGPLv2.1, see LICENSE at project root.
// URL: https://github.com/asrob-uc3m/robotDevastation

#ifndef __RD_NETWORK_MANAGER_HPP__
#define __RD_NETWORK_MANAGER_HPP__

#include <string>
#include <vector>
#include <map>

#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"
#include "NetworkEventListener.hpp"
#include "NetworkManager.hpp"
#include "MentalMapEventListener.hpp"

namespace rd{

/**
 * @ingroup NetworkLib
 *
 * @brief Manages the communications between the user and the RobotDevastation network
 *
 * NetworkManager is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the NetworkManager with getNetworkManager() and then we
 * access the manager with that reference.
 *
 * When the program finishes, the NetworkManager can be deallocated using destroyNetworkManager().
 *
 * Network events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. data that just arrived).
 *
 */
class NetworkManager: public MentalMapEventListener
{
    public:
        //------------------------------ Construction & destruction ---------------------------------------------------//
        /**
         * @brief Get a reference to the NetworkManager
         * @return By default, if no id is specified, this will return a reference to the first
         * NetworkManager that it can find in the registry, or NULL if no NetworkManager was registered.
         */
        static NetworkManager *getNetworkManager();

        /**
         * @brief Get a reference to the NetworkManager
         * @return The NetworkManager registered with the given id, NULL if the id is not found in
         * the registry.
         */
        static NetworkManager *getNetworkManager(const std::string & id);

        //! @brief Deallocate all the registered NetworkManager
        static bool destroyNetworkManager();

        virtual ~NetworkManager();


        //------------------------------ Manager Startup & Halting ----------------------------------------------------//
        /**
         * @brief Start the network manager
         *
         * This function is supposed to be called after NetworkManager configuration.
         */
        virtual bool start() = 0;

        //! @brief Stop the network manager
        virtual bool stop() = 0;

        //! @brief Returns true if manager is stopped (obviously)
        virtual bool isStopped() const = 0;


        //------------------------------ Configuration & Listeners ----------------------------------------------------//
        //! @brief Adds a NetworkEventListener to the list of observers to be notified of events
        bool addNetworkEventListener( NetworkEventListener * listener );

        //! @brief Unregisters all the NetworkEventListener stored
        bool removeNetworkEventListeners();

        //! @brief Configures a parameter with a value
        virtual bool configure(const std::string & parameter, const Player & value);



        //-- Network API
        //--------------------------------------------------------------------------------------------
        //! @brief Communicate to the network that a player has been hit by the user
        virtual bool sendPlayerHit(const Player & player, int damage) = 0;

        //! @brief Log the user into the network
        virtual bool login() = 0;

        //! @brief Log the user out of the network
        virtual bool logout() = 0;

        //! @brief Keeps the user connected to the server (requires to be called periodically)
        virtual bool keepAlive() = 0;

        //-- MentalMap listener API
        //-------------------------------------------------------------------------------------------
        //! @brief Implementation of MentalMapEventListener interface
        bool onTargetHit(const Target & target, const Player & player, const Weapon & weapon);

        //! @brief Implementation of MentalMapEventListener interface
        bool onRespawn(const Player & player);


protected:
        /**
         * @brief This function allows subclasses to install their unique instances in the singleton register to be
         * selected later by the user
         */
        static bool Register(NetworkManager *manager, const std::string & id);

        //! @brief Observers registered to be notified of network events
        std::vector<NetworkEventListener *> listeners;


    private:
        //! \brief Stores the unique instance of the NetworkManager
        static NetworkManager * networkManagerInstance;

        //! \brief Stores the id of the current unique instance used
        static std::string currentId;

        //! \brief Stores all the NetworkManager that have been registered
        static std::map< std::string, NetworkManager * > networkManagerRegistry;


};

}

#endif //-- __RD_NETWORK_MANAGER_HPP__
