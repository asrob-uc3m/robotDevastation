#include "StateMachineBuilder.hpp"

rd::StateMachineBuilder::StateMachineBuilder()
{
    //-- Default parameters:
    this->type = "YARP";
    this->initial_state_id = 0;
}

bool rd::StateMachineBuilder::setDirectorType(const std::string &type)
{
    this->type = type;
    return true;
}

bool rd::StateMachineBuilder::setInitialState(int initial_state_id)
{
    this->initial_state_id = initial_state_id;
    return true;
}

int rd::StateMachineBuilder::addState(rd::State *state)
{
    states.push_back(state);
    return states.size() -1;
}

bool rd::StateMachineBuilder::addTransition(int origin_id, int destination_id, int condition)
{
    //-- Create a pair with destination and condition
    std::pair<int, int> entry(destination_id, condition);

    //-- Check if entry for origin_id already exists
    if (transition_table.find(origin_id) != transition_table.end())
    {
        //-- Get the transitions for that entry:
        std::vector<std::pair<int, int> >* transition_vector;
        transition_vector = &transition_table[origin_id];

        //-- Add the entry:
        transition_vector->push_back(entry);
    }
    else
    {
        //-- Create the entry:
        std::vector<std::pair<int, int> > transition_vector;
        transition_vector.push_back(entry);

        //-- Add the entry to the table:
        transition_table[origin_id] = transition_vector;
    }

    return true;
}

rd::FiniteStateMachine *rd::StateMachineBuilder::buildStateMachine()
{
    //-- Create StateDirectors of selected type:
    for (int i = 0; i < states.size(); i++)
    {
        stateDirectors.push_back(createStateDirector(type, states[i]));
    }

    //-- Set transitions from table to state directors:
    for( std::map< int, std::vector<std::pair<int, int> > >::iterator it = transition_table.begin(); it != transition_table.end(); ++it)
    {
        //-- Extract data of current entry:
        int origin_id = it->first;
        if (origin_id >= stateDirectors.size())
        {
            RD_ERROR("Bad transition found: origin state %d does not exist.\n", origin_id);
            return NULL;
        }

        std::vector<std::pair<int, int> > transitions = it->second;

        //-- Apply transitions to stateDirector
        for (int i = 0; i < transitions.size(); i++)
        {
            int dest_id = transitions[i].first;
            if (dest_id >= stateDirectors.size())
            {
                RD_ERROR("Bad transition found: destination state %d does not exist.\n", dest_id);
                return NULL;
            }

            StateDirector * destination = stateDirectors[dest_id];
            stateDirectors[origin_id]->addTransition(destination, transitions[i].second);
        }
    }

    //-- Check that initial_state_id is within limits:
    if (initial_state_id >= stateDirectors.size())
    {
        RD_ERROR("Error: initial state provided (%d) does not exist.\n", initial_state_id);
        return NULL;
    }

    //-- Create fsm
    FiniteStateMachine * fsm = new FiniteStateMachine(stateDirectors, initial_state_id);

    return fsm;
}

rd::StateDirector *rd::StateMachineBuilder::createStateDirector(const std::string &type, State* state)
{
    if (type.compare("YARP") == 0)
    {
        return new YarpStateDirector(state);
    }
    else
    {
        //-- Default:
        return new YarpStateDirector(state);
    }
}