// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

#include "RdManagerDefault.hpp"

rdlib::RdManagerDefault::RdManagerDefault()
{
    this->managerStatus = MANAGER_STATUS_OK;

    //-- Enemy init:
    for( int i = 0; i < PIPELINE_SIZE; i++)
    {
        enemyPos.push_back( std::vector< std::pair<int, int> >() );
        enemySize.push_back( std::vector< double> () );
    }

}

bool rdlib::RdManagerDefault::callFunctionByName(const std::string &cmd)
{
    if(cmd == "toggleHeadTrack") {
        return this->toggleHeadTrack();
    } else {
        return RdManagerBase::callFunctionByName(cmd);
    }
}

bool rdlib::RdManagerDefault::setup()
{
    rdCameraBasePtr->getDimensions( width,  height, step);

    if ( ! RdManagerBase::setup() ) {
        return false;
    }

    rdImageProcessorBasePtr = new rdlib::RdImageProcessOpenCV();
    rdImageProcessorBasePtr->setup();

    return true;
}

bool rdlib::RdManagerDefault::askToStop()
{
    delete rdImageProcessorBasePtr;
    rdImageProcessorBasePtr = 0;
    return RdManagerBase::askToStop();
}

bool rdlib::RdManagerDefault::shoot() {
    if (!rdRobotBasePtr) return false;
    //
    return rdRobotBasePtr->shoot();
}

bool rdlib::RdManagerDefault::manage(int pipelineIndex)
{
    //-- Right now, it doesn't do much

    if ( managerStatus == MANAGER_STATUS_HEAD_TRACK )
    {
        RD_INFO("Tracking head.\n");
        //trackHead(pipelineIndex);
        rdImageProcessorBasePtr->process( rdCameraBasePtr->getBufferPtr(pipelineIndex), width, height, step,
                                          enemyPos[pipelineIndex], enemySize[pipelineIndex]);
    }
}

bool rdlib::RdManagerDefault::toggleHeadTrack()
{
    if ( managerStatus != MANAGER_STATUS_HEAD_TRACK )
        managerStatus = MANAGER_STATUS_HEAD_TRACK;
    else
        managerStatus = MANAGER_STATUS_OK;
}

