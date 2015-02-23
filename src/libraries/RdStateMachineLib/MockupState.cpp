#include "MockupState.hpp"

const std::string rd::MockupState::debug_port_name = "/testState";


rd::MockupState::MockupState(int id)
{
    this->id = id;
    internal_variable = -1;
    first_loop = true;

    commandPort = dynamic_cast<yarp::os::BufferedPort<yarp::os::Bottle> *>(this);

    if (!startNetwork(id))
    {
        RD_ERROR("Could not start network\n");
        return;
    }
}

rd::MockupState::~MockupState()
{
    closeNetwork();
}

bool rd::MockupState::setup()
{
    RD_INFO("State with id %d entered in setup() function\n", id);
    yarp::os::Bottle outMsg;
    outMsg.addString("setup");
    debugPort.write(outMsg);

    internal_variable = 0;
}

bool rd::MockupState::loop()
{
    RD_INFO("State with id %d entered in loop() function\n", id);
    if(first_loop)
    {
        yarp::os::Bottle outMsg;
        outMsg.addString("loop");
        debugPort.write(outMsg);
        first_loop = false;
    }
}

bool rd::MockupState::cleanup()
{
    RD_INFO("State with id %d entered in cleanup() function\n", id);
    yarp::os::Bottle outMsg;
    outMsg.addString("cleanup");
    debugPort.write(outMsg);

    internal_variable = -1;
    first_loop = true;
}

int rd::MockupState::evaluateConditions()
{
    return internal_variable;
}

void rd::MockupState::onRead(yarp::os::Bottle &b)
{
    int new_status = b.get(0).asInt();
    internal_variable = new_status;

    RD_INFO("Received: %d\n", new_status);
}

bool rd::MockupState::startNetwork(int id)
{
    //-- Init yarp network & server
    yarp::os::Network::init();
    //yarp::os::Network::runNameServer(argc, argv);

    //-- Setup yarp ports
    std::stringstream debug_port_ss;
    debug_port_ss << debug_port_name << "/" << id;

    if (!debugPort.open(debug_port_ss.str() + "/status:o"))
        return false;

    if(!commandPort->open(debug_port_ss.str() + "/command:i"))
        return false;

    return true;
}

void rd::MockupState::closeNetwork()
{
    RD_INFO("Closing ports...\n");

    //-- Close yarp ports
    debugPort.interrupt();
    commandPort->interrupt();

    debugPort.close();
    commandPort->close();

    yarp::os::Network::fini();
}
