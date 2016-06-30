#include "RdMentalMap.hpp"

//-- This is very important:
rd::RdMentalMap * rd::RdMentalMap::mentalMapInstance = NULL;

rd::RdMentalMap::RdMentalMap()
{
    current_weapon = 0;
    SDLAudioManager::RegisterManager();
    audioManager = AudioManager::getAudioManager("SDL");
}

rd::RdMentalMap *rd::RdMentalMap::getMentalMap()
{
    if (mentalMapInstance == NULL)
        mentalMapInstance = new RdMentalMap();

    return mentalMapInstance;
}

bool rd::RdMentalMap::destroyMentalMap()
{
    if (mentalMapInstance == NULL)
        return false;

    delete mentalMapInstance;
    mentalMapInstance = NULL;

    return true;
}

bool rd::RdMentalMap::configure(const int &player_id)
{
    this->my_id = player_id;
    this->myself = NULL;
    return true;
}

std::vector<rd::RdTarget> rd::RdMentalMap::getTargets()
{
    std::vector<RdTarget> target_vector;

    for( std::map<int, RdTarget>::const_iterator it = targets.begin(); it != targets.end(); ++it)
    {
        target_vector.push_back(it->second);
    }

    return target_vector;

}

std::vector<rd::RdPlayer> rd::RdMentalMap::getPlayers()
{
    std::vector<RdPlayer> player_vector;

    for( std::map<int, RdPlayer>::const_iterator it = players.begin(); it != players.end(); ++it)
    {
        player_vector.push_back(it->second);
    }

    return player_vector;
}

rd::RdTarget rd::RdMentalMap::getTarget(const int &id)
{
    if ( targets.find(id) != targets.end() )
    {
        return targets[id];
    }
    else
    {
        RD_ERROR("Enemy with id %i not found!!\n", id);
        RD_ERROR("Returning standard target\n");
        return RdTarget();
    }
}

rd::RdPlayer rd::RdMentalMap::getPlayer(const int &id)
{
    if ( players.find(id) != players.end() )
    {
        return players[id];
    }
    else
    {
        RD_ERROR("Player with id %i not found!!\n", id);
        RD_ERROR("Returning standard player\n");
        return RdPlayer();
    }
}

rd::RdPlayer rd::RdMentalMap::getMyself()
{
    if (!myself)
    {
        RD_ERROR("Myself not found in mental map\n");
        RD_ERROR("Returning standard player\n");
        return RdPlayer();
    }
    else
    {
        return *myself;
    }
}

rd::RdWeapon rd::RdMentalMap::getCurrentWeapon()
{
    if ( (int)weapons.size() >= current_weapon)
        return weapons[current_weapon];
    else
    {
        RD_ERROR("No weapons added yet!\n");
        RD_ERROR("Returning standard weapon\n");
        return RdWeapon();
    }
}

void rd::RdMentalMap::addWeapon(RdWeapon weapon)
{
    weapons.push_back(weapon);
}

bool rd::RdMentalMap::shoot()
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
        for( std::map<int, RdTarget>::iterator it = targets.begin(); it != targets.end(); ++it)
        {
            if ( weapons[current_weapon].canShootTarget(it->second) )
            {
                //-- Decrease player's life:
                RdPlayer * player = &players[it->second.getPlayerId()];
                RD_SUCCESS("Target %s was hit!\n", player->getName().c_str());

                player->getDamageFromWeapon(weapons[current_weapon]);

                hit = true;

                //-- Update listeners
                for (size_t i = 0; i < listeners.size(); i++)
                    listeners[i]->onTargetHit(it->second, *player, weapons[current_weapon]);

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

bool rd::RdMentalMap::reload()
{
    audioManager->play("reload", false);
    return weapons[current_weapon].reload();
}

bool rd::RdMentalMap::updatePlayers(std::vector<rd::RdPlayer> new_player_vector)
{
    //-- Right now, this just replaces the players inside the mental map

    //-- Clear the pointer to myself (player)
    myself = NULL;

    for (int i = 0; i < (int) new_player_vector.size(); i++)
    {
        int id = new_player_vector[i].getId();
        players[id] = new_player_vector[i];

        if ( id == my_id)
            myself = &players[id];
    }

    return true;
}

bool rd::RdMentalMap::updateTargets(std::vector<RdTarget> new_target_detections)
{
    //-- Reduce the belief of all the targets and deletes them when belief is 0
    for( std::map<int, RdTarget>::iterator it = targets.begin(); it != targets.end(); ++it)
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

bool rd::RdMentalMap::respawn()
{
    return false;
}

bool rd::RdMentalMap::addMentalMapEventListener(rd::RdMentalMapEventListener *listener)
{
    listeners.push_back(listener);
    return true;
}

bool rd::RdMentalMap::removeMentalMapEventListeners()
{
    listeners.clear();
    return true;
}

bool rd::RdMentalMap::onDataArrived(std::vector<rd::RdPlayer> players)
{
    return updatePlayers(players);
}
