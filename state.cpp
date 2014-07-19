#include "state.h"

// Creates a new state and adds it to the given set of states.
State::State(unordered_map<int, State*>* states)
{
    this->isAccepting = false;
    // Gives a unique ID to the set.
    this->id = State::idCounter++;
    states->emplace(this->id, this);
}

int State::getId()
{
    return this->id;
}

// Adds a new destination from the current state to the given state.
// The destinations are defined on a unordered_map on each state,
// and use the state pointer as the key and the symbol as the value.
void State::addDestination(State* destination, char symbol)
{

    this->destinations.emplace(destination, symbol);
}

int State::idCounter = 0;
