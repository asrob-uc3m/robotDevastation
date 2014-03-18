#include "RdManagerBase.hpp"

rdlib::RdManagerBase::RdManagerBase()
{
    //- Init the references to other modules
    rdCameraBasePtr = 0;
    rdInputBasePtr = 0;
    rdOutputBasePtr = 0;
    rdRobotBasePtr = 0;

    //-- Add shoot to dictionary
    functionMap[ "shoot"] = (void *) &shootWrapper;

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

bool rdlib::RdManagerBase::setup()
{
    RD_INFO("RdManagerBase: setup!\n");

    //-- Link input to the manager
    rdInputBasePtr->setRdManagerBasePtr(this);

    //-- Link output to the manager
    rdOutputBasePtr->setRdManagerBasePtr(this);

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

    int result = pthread_create( &processImage_thread, NULL, processImageThread, (void *) this );
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
    RD_INFO("RdCameraBase: exiting...\n");

    pthread_join( processImage_thread, NULL );

    //-- Delete semaphores
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

void *rdlib::RdManagerBase::getFunctionByName( std::string function_name ) {
    return functionMap[ function_name ];
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

void * rdlib::RdManagerBase::processImageThread(void *This)
{
    (( rdlib::RdManagerBase *) This)->processImage();
}
