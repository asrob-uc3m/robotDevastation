// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#ifndef __RD_MENTAL_MAP_HPP__
#define __RD_MENTAL_MAP_HPP__

#include <map>
#include <vector>
#include <iterator>

#include "Player.hpp"
#include "Target.hpp"
#include "Weapon.hpp"
#include "Utils.hpp"
#include "MentalMapEventListener.hpp"
#include "NetworkEventListener.hpp"
#include "NetworkManager.hpp"
#include "AudioManager.hpp"

namespace rd{

/**
 * @ingroup rd_libraries
 *
 * \defgroup RdMentalMapLib
 *
 * @brief The RdMentalMapLib library contains all the classes related to game information storage (e.g. players, targets, weapons, etc)
 */

/**
 * @ingroup RdMentalMapLib
 *
 * @brief Mental map is a repository for information about the players, targets and weapons.
 *
 * RdMentalMap is a <a href="http://en.wikipedia.org/wiki/Singleton_pattern">singleton text</a> (only
 * one instance of this object can exist, that is is shared by all the users). To use this
 * class, we first get the reference to the RdMentalMap with getMentalMap() and then we
 * access the RdMentalMap with that reference.
 *
 * When the program finishes, the RdMentalMap can be deallocated using destroyMentalMap().
 *
 * RdMentalMap implements the RdNetworkEventListener <a href="http://en.wikipedia.org/wiki/Observer_pattern">observer</a> interface,
 * which allows this class to receive notification of network events to update its local data.
 *
 * Data change events are broadcasted to the registered <a href="http://en.wikipedia.org/wiki/Observer_pattern">listeners</a>,
 * along with the data relevant to the event triggered (i.e. the new targets detected).
 *
 */
class MentalMap : public NetworkEventListener
{
    public:
        //-- Creation and configuration
        //--------------------------------------------------------------------------------------------
        //! @brief Get a reference to the RdMentalMap
        static MentalMap * getMentalMap();

        //! @brief Get a reference to the RdMentalMap
        static bool destroyMentalMap();

        //! @brief Store the id of the player corresponding to the user
        bool configure(const int& player_id );


        //-- Interface to get data
        //--------------------------------------------------------------------------------------------
        std::vector<Target> getTargets();
        std::vector<Player> getPlayers();
        Target getTarget(const int& id = -1);
        Player getPlayer(const int& id = -1);

        //! @brief Get the player corresponding to the user
        Player getMyself();


        //-- Weapon interface
        //--------------------------------------------------------------------------------------------
        void addWeapon(Weapon weapon);
        Weapon getCurrentWeapon();

        //! @brief Manage all the actions to be carried out when the user shoots (sound, update players, etc)
        bool shoot();

        //! @brief Manage all the actions to be carried out when the user reloads (sound, update weapons, etc)
        bool reload();


        //-- Functions to update data
        //--------------------------------------------------------------------------------------------
        //! @brief  The current implementation just replaces the players inside the mental map by the new players
        bool updatePlayers(std::vector<Player> new_player_vector);

        /**
         * @brief Update the targets stored in the mental map
         *
         * If a target previously detected is no longer present in the new detections, decreases the belief value
         * until reaching 0. Then, it deletes that target.
         */
        bool updateTargets(std::vector<Target> new_target_detections);

        //! @brief Restores the health of current player (and does more stuff if needed)
        bool respawn();


        //-- Listeners
        //--------------------------------------------------------------------------------------------
        //! @brief Adds a RdMentalMapEventListener to the list of observers to be notified of events
        bool addMentalMapEventListener( MentalMapEventListener * listener);

        //! @brief Unregisters all the RdMentalMapEventListener stored
        bool removeMentalMapEventListeners();

    private:
        /**
         * @brief Constructor
         *
         * Constructor for this class is private, since the singleton can only be instantiated once,
         * and the instantiation is done at the getMentalMap() method
         */
        MentalMap();

        //! @brief Stores the unique instance of the RdMentalMap
        static MentalMap * mentalMapInstance;


        //! @brief Storage for the target data. The key of the dictionary is the player id.
        std::map<int, Target> targets;

        //! @brief Storage for the players data. The key of the dictionary is the player id.
        std::map<int, Player> players;

        //! @brief Storage for the weapons data.
        std::vector<Weapon> weapons;

        //! @brief Index of the weapon currenly selected.
        int current_weapon;


        //! @brief Id of the player corresponding to the user
        int my_id;

        //! @brief Pointer to the player corresponding to the user
        Player* myself;


        //! @brief Reference to the AudioManager
        AudioManager * audioManager;


        //! @brief Observers registered to be notified of data change events
        std::vector<MentalMapEventListener *> listeners;


        //-- Implementation of RdNetworkEventListener functions
        //--------------------------------------------------------------------------------------------
        //! @brief Updates the local information about the players with the new data received by the RdNetworkManager.
        bool onDataArrived(std::vector<Player> players);
};

}

#endif //__RD_MENTAL_MAP_HPP__
