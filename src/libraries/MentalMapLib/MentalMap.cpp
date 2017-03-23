#include "MentalMap.hpp"

//-- This is very important:
rd::MentalMap * rd::MentalMap::mentalMapInstance = NULL;

rd::MentalMap::MentalMap()
{
    current_weapon = 0;
    SDLAudioManager::RegisterManager();
    audioManager = AudioManager::getAudioManager("SDL");
}

rd::MentalMap *rd::MentalMap::getMentalMap()
{
    if (mentalMapInstance == NULL)
        mentalMapInstance = new MentalMap();

    return mentalMapInstance;
}

bool rd::MentalMap::destroyMentalMap()
{
    if (mentalMapInstance == NULL)
        return false;

    delete mentalMapInstance;
    mentalMapInstance = NULL;

    return true;
}

bool rd::MentalMap::configure(const int &player_id)
{
    this->my_id = player_id;
    this->myself = NULL;
    return true;
}

std::vector<rd::Target> rd::MentalMap::getTargets()
{
    std::vector<Target> target_vector;

    for( std::map<int, Target>::const_iterator it = targets.begin(); it != targets.end(); ++it)
    {
        target_vector.push_back(it->second);
    }

    return target_vector;

}

std::vector<rd::Player> rd::MentalMap::getPlayers()
{
    std::vector<Player> player_vector;

    for( std::map<int, Player>::const_iterator it = players.begin(); it != players.end(); ++it)
    {
        player_vector.push_back(it->second);
    }

    return player_vector;
}

rd::Target rd::MentalMap::getTarget(const int &id)
{
    if ( targets.find(id) != targets.end() )
    {
        return targets[id];
    }
    else
    {
        RD_ERROR("Enemy with id %i not found!!\n", id);
        RD_ERROR("Returning standard target\n");
        return Target();
    }
}

rd::Player rd::MentalMap::getPlayer(const int &id)
{
    if ( players.find(id) != players.end() )
    {
        return players[id];
    }
    else
    {
        RD_ERROR("Player with id %i not found!!\n", id);
        RD_ERROR("Returning standard player\n");
        return Player();
    }
}

rd::Player rd::MentalMap::getMyself()
{
    if (!myself)
    {
        RD_ERROR("Myself not found in mental map\n");
        RD_ERROR("Returning standard player\n");
        return Player();
    }
    else
    {
        return *myself;
    }
}

rd::Weapon rd::MentalMap::getCurrentWeapon()
{
    if ( (int)weapons.size() >= current_weapon)
        return weapons[current_weapon];
    else
    {
        RD_ERROR("No weapons added yet!\n");
        RD_ERROR("Returning standard weapon\n");
        return Weapon();
    }
}

void rd::MentalMap::addWeapon(Weapon weapon)
{
    weapons.push_back(weapon);
}

bool rd::MentalMap::shoot()
{
    bool hit = false;
    int current_ammo = weapons[current_weapon].getCurrentAmmo();
    if ( current_ammo > 0)
    {
        //-- Play sound
        audioManager->play("shoot", false);

        //-- Decrease ammo
        current_ammo--;
        weapons[current_weapon].setCurrentAmmo(current_ammo);

        //-- Check for collision with any target:
        for( std::map<int, Target>::iterator it = targets.begin(); it != targets.end(); ++it)
        {
            if ( weapons[current_weapon].canShootTarget(it->second) )
            {
                //-- Decrease player's life:
                int target_id = it->second.getPlayerId();
                if (players.find(target_id) != players.end())
                {
                    Player * player = &players[target_id];
                    RD_SUCCESS("Target %s was hit!\n", player->getName().c_str());

                    player->getDamageFromWeapon(weapons[current_weapon]);

                    hit = true;

                    //-- Update listeners
                    for (size_t i = 0; i < listeners.size(); i++)
                        listeners[i]->onTargetHit(it->second, *player, weapons[current_weapon]);
                }
                else
                {
                    RD_WARNING("Ignoring target with invalid player id %d\n", target_id);
                }

            }
            else
            {
                RD_INFO("Missed!\n");
            }
        }
    }
    else
    {
            //-- Play sound
            audioManager->play("noAmmo", false);
            RD_WARNING("No ammo! Reload!\n");
    }

    return hit;
}

bool rd::MentalMap::reload()
{
    audioManager->play("reload", false);
    return weapons[current_weapon].reload();
}

bool rd::MentalMap::updatePlayers(std::vector<rd::Player> new_player_vector)
{
    //-- Right now, this just replaces the players inside the mental map

    //-- Clear the pointer to myself (player)
    myself = NULL;
    players.clear();

    for (int i = 0; i < (int) new_player_vector.size(); i++)
    {
        int id = new_player_vector[i].getId();
        players[id] = new_player_vector[i];

        if ( id == my_id)
            myself = &players[id];
    }

    return true;
}

bool rd::MentalMap::updateTargets(std::vector<Target> new_target_detections)
{
    //-- Reduce the belief of all the targets and deletes them when belief is 0
    for( std::map<int, Target>::iterator it = targets.begin(); it != targets.end(); ++it)
    {
        if ( !it->second.reduceBelief(10) )
            targets.erase(it);
    }

    //-- Add new targets / update old ones (just replacing):
    for (int i = 0; i < (int) new_target_detections.size(); i++)
    {
        int id = new_target_detections[i].getPlayerId();
        targets[id] = new_target_detections[i];
    }

    return true;
}

bool rd::MentalMap::respawn()
{
    myself->setHealth(myself->getMaxHealth());

    //-- Update listeners
    for (size_t i = 0; i < listeners.size(); i++)
        listeners[i]->onRespawn(*myself);

    return true;
}

bool rd::MentalMap::addMentalMapEventListener(rd::MentalMapEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::MentalMap::removeMentalMapEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::MentalMap::onDataArrived(std::vector<rd::Player> players)
{
    return updatePlayers(players);
}
