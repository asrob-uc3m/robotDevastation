// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

rdlib::RdManagerDefault::RdManagerDefault()
{
    this->managerStatus = MANAGER_STATUS_OK;

    //-- Enemy init:
    for( int i = 0; i < PIPELINE_SIZE; i++)
    {
        //enemyPos.push_back( std::vector< std::pair<int, int> >() );
        //enemySize.push_back( std::vector< double> () );
    }

    //-- Weapons init:
    weapons.push_back(RdWeaponBase(RdWeaponBase::RD_WEAPON_LASER_GUN));
    weapons.push_back(RdWeaponBase(RdWeaponBase::RD_WEAPON_MACHINE_GUN));
    currentWeapon = &weapons[0];
    currentWeaponIndex = 0;
}

bool rdlib::RdManagerDefault::callFunctionByName(const std::string &cmd)
{
    RD_DEBUG("Passing cmd \"%s\" to parent handler!\n", cmd.c_str());
    RdManagerBase::callFunctionByName(cmd);

    if (cmd == "toggleHeadTrack")
        this->toggleHeadTrack();
    else if ( cmd == "shoot")
        this->shoot();
}

bool rdlib::RdManagerDefault::setup()
{
    rdCameraBasePtr->getDimensions( width,  height, step);

    if ( ! RdManagerBase::setup() ) {
        return false;
    }

    //rdImageProcessorBasePtr = new rdlib::RdImageProcessOpenCV();  // Tracks faces
    rdImageProcessorBasePtr = new rdlib::RdImageProcessZbar();  // Tracks QR codes
    rdImageProcessorBasePtr->setup();

    return true;
}


bool rdlib::RdManagerDefault::askToStop()
{
    delete rdImageProcessorBasePtr;
    rdImageProcessorBasePtr = 0;
    return RdManagerBase::askToStop();
}

bool rdlib::RdManagerDefault::shoot()
{
    if (!rdRobotBasePtr)
        return false;

    if (currentWeapon && currentWeapon->getCurrentAmmo() > 0 )
        return rdRobotBasePtr->shoot();
    else
        return false;
}

bool rdlib::RdManagerDefault::manage(int pipelineIndex)
{
    //-- Right now, it doesn't do much

    if ( managerStatus == MANAGER_STATUS_HEAD_TRACK )
    {
        RD_INFO("Tracking head.\n");
        //trackHead(pipelineIndex);
        rdImageProcessorBasePtr->process( rdCameraBasePtr->getBufferPtr(pipelineIndex), width, height, step,
                                          vectorOfRdEnemy, mutexOfVectorOfRdEnemy);
    }
}

bool rdlib::RdManagerDefault::toggleHeadTrack()
{
    if ( managerStatus != MANAGER_STATUS_HEAD_TRACK )
        managerStatus = MANAGER_STATUS_HEAD_TRACK;
    else
        managerStatus = MANAGER_STATUS_OK;
}

