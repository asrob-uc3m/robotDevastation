#include "MockupState.hpp"

const unsigned int rd::MockupState::STATE_INITIAL = 0;
const unsigned int rd::MockupState::STATE_SETUP = 1;
const unsigned int rd::MockupState::STATE_LOOP = 2;
const unsigned int rd::MockupState::STATE_CLEANUP = 4;

const unsigned int rd::MockupState::REQUEST_STATE = 0;

rd::MockupState::MockupState(int id)
{
    //-- Set class parameters:
    this->id = id;

    std::stringstream ss;
    ss << "MockupState #" << id;
    state_id = ss.str();

    internal_variable = -1;
    state_history = STATE_INITIAL;

    RD_DEBUG("Creating MockupState with id: %s\n", state_id.c_str());
}

rd::MockupState::~MockupState()
{
}

bool rd::MockupState::setup()
{
    RD_INFO("State with id %d entered in setup() function\n", id);

    internal_variable = 0;
    state_history |= STATE_SETUP;

    return true;
}

bool rd::MockupState::loop()
{
    RD_INFO("State with id %d entered in loop() function\n", id);
    if (!(state_history & STATE_LOOP))
    {
        RD_DEBUG("First loop (id %d)\n", id);
        state_history |= STATE_LOOP;
    }

    return true;
}

bool rd::MockupState::cleanup()
{
    RD_INFO("State with id %d entered in cleanup() function\n", id);

    internal_variable = -1;
    state_history |= STATE_CLEANUP;

    return true;
}

int rd::MockupState::evaluateConditions()
{
    return internal_variable;
}

bool rd::MockupState::read(yarp::os::ConnectionReader & connection)
{
    yarp::os::Bottle in;
    in.read(connection);
    int received = in.get(0).asInt();

    if (received != REQUEST_STATE)
    {
        RD_INFO("Received: %d at state %s\n", received, state_id.c_str());
        internal_variable = received;
    }

    yarp::os::ConnectionWriter *returnToSender = connection.getWriter();

    if (returnToSender != NULL)
    {
        RD_INFO("Preparing response...\n");
        yarp::os::Bottle out;
        out.addInt(state_history);
        out.write(*returnToSender);
    }

    return true;
}
