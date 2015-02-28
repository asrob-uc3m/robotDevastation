#include "MockupState.hpp"

const std::string rd::MockupState::debug_port_name = "/testState";


rd::MockupState::MockupState(int id)
{
    //-- Set class parameters:
    this->id = id;

    std::stringstream ss;
    ss << "MockupState #" << id;
    state_id = ss.str();

    internal_variable = -1;
    first_loop = true;

    commandPort = dynamic_cast<yarp::os::BufferedPort<yarp::os::Bottle> *>(this);

    RD_DEBUG("Creating MockupState with id: %s\n", state_id.c_str());

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

    internal_variable = 0;

    yarp::os::Bottle& outMsg = debugPort.prepare();
    outMsg.clear();
    outMsg.addString("setup");
    debugPort.writeStrict();

    return true;
}

bool rd::MockupState::loop()
{
    RD_INFO("State with id %d entered in loop() function\n", id);
    if(first_loop)
    {
        RD_DEBUG("First loop (id %d)\n", id);
        first_loop = false;

        yarp::os::Bottle& outMsg = debugPort.prepare();
        outMsg.clear();
        outMsg.addString("loop");
        debugPort.writeStrict();
    }

    return true;
}

bool rd::MockupState::cleanup()
{
    RD_INFO("State with id %d entered in cleanup() function\n", id);

    internal_variable = -1;
    first_loop = true;

    yarp::os::Bottle& outMsg = debugPort.prepare();
    outMsg.clear();
    outMsg.addString("cleanup");
    debugPort.writeStrict();

    return true;
}

int rd::MockupState::evaluateConditions()
{
    return internal_variable;
}

void rd::MockupState::onRead(yarp::os::Bottle &b)
{
    if (internal_variable != -1)
    {
        int new_status = b.get(0).asInt();
        internal_variable = new_status;

        RD_INFO("Received: %d at state %s\n", new_status, state_id.c_str());
    }

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
    {
        RD_ERROR("Could not open yarp port: %s\n", (debug_port_ss.str() + "/status:o").c_str() );
        return false;
    }

    if(!commandPort->open(debug_port_ss.str() + "/command:i"))
    {
        RD_ERROR("Could not open yarp port: %s\n", (debug_port_ss.str() +"/command:i").c_str() );
        return false;
    }
    commandPort->useCallback(*commandPort);

    return true;
}

void rd::MockupState::closeNetwork()
{
    RD_INFO("Closing ports...\n");

    //-- Close yarp ports
    commandPort->disableCallback();

    debugPort.interrupt();
    commandPort->interrupt();

    debugPort.close();
    commandPort->close();

    yarp::os::Network::fini();
}
