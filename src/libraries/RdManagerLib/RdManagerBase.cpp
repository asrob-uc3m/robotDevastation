#include "RdManagerBase.hpp"

rdlib::RdManagerBase::RdManagerBase()
{
    //- Init the references to other modules
    rdCameraBasePtr = 0;
    rdInputBasePtr = 0;
    rdOutputBasePtr = 0;
    rdRobotBasePtr = 0;
    rdImageProcessorBasePtr = 0;

    //-- Init the semaphores
    captureSemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( captureSemaphores+i, 0, 1);

    processSemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( processSemaphores+i, 0, 0);

    displaySemaphores = new sem_t[PIPELINE_SIZE];
    for( int i = 0; i < PIPELINE_SIZE; i++)
        sem_init( displaySemaphores+i, 0, 0);
}

bool rdlib::RdManagerBase::callFunctionByName(const std::string& cmd) {
    if ( cmd  == "quit" || cmd == "exit"  || cmd == "askToStop")
    {
        this->askToStop();
    }
    else if( cmd == "shoot" )
    {
        this->shoot();
    }
    else
    {
        rdRobotBasePtr->callFunctionByName(cmd);
    }
}


bool rdlib::RdManagerBase::setup()
{
    RD_INFO("RdManagerBase: setup!\n");

    //-- Link input to the manager
    rdInputBasePtr->setRdManagerBasePtr(this);

    //-- Link output to the manager & camera
    rdOutputBasePtr->setRdManagerBasePtr(this);
    rdOutputBasePtr->setRdCameraBasePtr(rdCameraBasePtr);

    //-- Semaphore configuration
    rdCameraBasePtr->setCaptureSemaphores(captureSemaphores);
    rdCameraBasePtr->setProcessSemaphores(processSemaphores);

    rdOutputBasePtr->setDisplaySemaphores(displaySemaphores);
    rdOutputBasePtr->setCaptureSemaphores(captureSemaphores);

    rdCameraBasePtr->setup();
    rdOutputBasePtr->setup();

    return true;
}

bool rdlib::RdManagerBase::start()
{
    //-- Start the different modules in order:
    rdCameraBasePtr->start();

    int result = pthread_create( &manage_thread, NULL, manageThread, (void *) this );
    if (result == 0)
        { RD_INFO("RdManagerBase started thread.\n");}
    else
        { RD_WARNING("RdManagerBase could not create thread.\n");}

    rdOutputBasePtr->start();

    return true;
}

bool rdlib::RdManagerBase::askToStop()
{
    RD_INFO("RdManagerBase: stopping...\n");

    managerStatus = MANAGER_STATUS_STOPPED;

    rdCameraBasePtr->askToStop();
    rdOutputBasePtr->askToStop();

    return true;
}


bool rdlib::RdManagerBase::quit()
{
    RD_INFO("RdManagerBase: stopping different components...\n");

    rdCameraBasePtr->quit();
    rdOutputBasePtr->quit();

    RD_INFO("RdManagerBase: waiting for the manager thread to finish...\n");
    pthread_join( manage_thread, NULL );

    //-- Delete semaphores
    RD_INFO("RdManagerBase: deleting semaphores...\n");

    delete[] captureSemaphores;
    captureSemaphores = 0;
    delete[] processSemaphores; //-- Doing this here will cause (probably) a segmentation fault
    processSemaphores = 0;
    delete[] displaySemaphores;
    displaySemaphores = 0;

    return true;
}

bool rdlib::RdManagerBase::shootWrapper(void *This)
{
    return (( rdlib::RdManagerBase *) This)->shoot();
}

int rdlib::RdManagerBase::getManagerStatus() {
    return managerStatus;
}

void rdlib::RdManagerBase::setRdCameraBasePtr(RdCameraBase* rdCameraBasePtr ) {
    this->rdCameraBasePtr = rdCameraBasePtr;
}
void rdlib::RdManagerBase::setRdInputBasePtr(RdInputBase* rdInputBasePtr ) {
    this->rdInputBasePtr = rdInputBasePtr;
}
void rdlib::RdManagerBase::setRdOutputBasePtr(RdOutputBase* rdOutputBasePtr ) {
    this->rdOutputBasePtr = rdOutputBasePtr;
}
void rdlib::RdManagerBase::setRdRobotBasePtr(RdRobotBase* rdRobotBasePtr ) {
    this->rdRobotBasePtr = rdRobotBasePtr;
}


rdlib::RdCameraBase* rdlib::RdManagerBase::getRdCameraBasePtr() {
    return this->rdCameraBasePtr;
}

void rdlib::RdManagerBase::getEnemies( int pipelineIndex,  std::vector< std::pair<int, int> >& enemyPos,
                 std::vector< double >& enemySize) {
    enemyPos = this->enemyPos[pipelineIndex];
    enemySize = this->enemySize[pipelineIndex];
}

void * rdlib::RdManagerBase::manageThread(void *This)
{
    (( rdlib::RdManagerBase *) This)->manageWithSync();
}

bool rdlib::RdManagerBase::manageWithSync()
{
    if (managerStatus == MANAGER_STATUS_OK)
    {
        do
        {
            for (int i = 0; i < PIPELINE_SIZE; i++)
            {
                //-- Lock the semaphore
                sem_wait( processSemaphores+i);

                //RD_INFO("Processed frame #%i\n", i);
                manage(i);

                //-- Unlock the corresponding process semaphore
                sem_post(displaySemaphores+i);
            }
        } while(managerStatus != MANAGER_STATUS_STOPPED);
    }
    RD_SUCCESS("Exited manager main thread!\n");
}
