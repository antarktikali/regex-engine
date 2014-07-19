#include <QDebug>
#include <stdlib.h>
#include <algorithm>
#include <iterator>
#include <stack>
#include "nfa.h"


NFA::NFA() {

}

// Checks whether this automaton accepts 's'.
bool NFA::accepts(string str) {
    unordered_map<int, State*> P;
    unordered_map<int, State*> T;

    while(true)
    {
        P.emplace(this->initialState->getId(), this->initialState);

        do
        {
            T = P;

            for(auto& state : P)
            {
                unordered_map<int, State*> epsilonSet;
                epsilonSet = this->epsilonClosure(state.second, nullptr);

                for(auto& epsilonState : epsilonSet)
                {
                    P.emplace(epsilonState.first, epsilonState.second);
                }
            }
        } while(T != P);

        if(this->hasAcceptingStates(&T))
        {
            break;
        }

        if(str.size() <= 0)
        {
            break;
        }

        char c = str[0];
        str = str.substr(1);

        T.clear();
        for(auto& state : P)
        {
            for(auto& destinations : state.second->destinations)
            {
                if(destinations.second == c)
                {
                    T.emplace(destinations.first->getId(), destinations.first);
                }
            }
        }

        if(this->hasAcceptingStates(&T))
        {
            break;
        }
        P = T;
    }

    if(this->hasAcceptingStates(&T)) {
        return true;
    } else {
        return false;
    }
}

// Checks if the given set has an accepting state.
bool NFA::hasAcceptingStates(unordered_map<int, State*>* set)
{
    for(auto& kv : *set)
    {
        if(kv.second->isAccepting)
        {
            return true;
        }
    }
    return false;
}

// Returns all of the states which could be reached with epsilon.
// The function is called recursively until a non-epsilon
// destination is found.
unordered_map<int, State*> NFA::epsilonClosure(State* state_,
                                unordered_map<int, State*>* epsilonNeighbours) {
    // If the function is called for the first time, the set for the epsilon
    // neighbours would be null, so create a new set.
    if(nullptr == epsilonNeighbours)
    {
        epsilonNeighbours = new unordered_map<int, State*>();
    }

    // Add the state itself to the set.
    epsilonNeighbours->emplace(state_->getId(), state_);

    // Loop over the destinations from this state
    for(auto& destinations : state_->destinations)
    {
        // If the destination is a epsilon edge, continue doing the same.
        // We should check if the destination has an epsilon to the state itsef,
        // otherwise we enter a endless loop.
        if(destinations.second == State::EPSILON &&
            destinations.first->getId() != state_->getId())
        {
            // Recursively call the function, sending the destination state
            // and the pointer to the neighbours set.
            epsilonClosure(destinations.first, epsilonNeighbours);
        }
    }

    // Return the set
    return *epsilonNeighbours;
}

// Constructs a NFA accepting a single symbol
NFA NFA::singleSymbol(char c) {
    // Create a new NFA
    NFA nfa;

    // Add two states, one initial and one final.
    State* initialState = new State(&(nfa.states));
    State* finalState = new State(&(nfa.states));

    // Set the final state as accepting.
    finalState->isAccepting = true;

    // Connect the first state to the final state with the given character 'c'
    initialState->addDestination(finalState, c);

    nfa.initialState = initialState;

    return nfa;
}

// Constructs a NFA from the union of two given NFA's.
NFA NFA::unionOfNFAs(NFA& nfa1, NFA& nfa2) {
    NFA nfa;

    // Add the states of the given NFA's to the new NFA.
    nfa.states = nfa1.states;
    for(auto& kv : nfa2.states)
    {
        nfa.states.emplace(kv.first, kv.second);
    }

    // Add a new initial state
    State* initialState = new State(&(nfa.states));
    nfa.initialState = initialState;

    // Connect the initial state to the initial states of the two given NFA's.
    initialState->addDestination(nfa1.initialState, State::EPSILON);
    initialState->addDestination(nfa2.initialState, State::EPSILON);

    return nfa;
}

// Constructs a NFA from the concatenation of the two given NFA's.
NFA NFA::concatenate(NFA& nfa1, NFA& nfa2) {
    NFA nfa;

    // Add the states of the given NFA's to the new NFA.
    nfa.states = nfa1.states;
    for(auto& kv : nfa2.states)
    {
        nfa.states.emplace(kv.first, kv.second);
    }

    // Set the first NFA's initial state as the initial state of the new NFA.
    nfa.initialState = nfa1.initialState;

    // Connect the accepting states of the first NFA to the initial state of the
    // second NFA.
    for(auto& state : nfa1.states)
    {
        if(state.second->isAccepting)
        {
            // The final states of the first NFA should not be accepting anymore
            state.second->isAccepting = false;
            state.second->addDestination(nfa2.initialState, State::EPSILON);
        }
    }

    return nfa;
}

// Constructs a NFA with star notation from the given NFA.
NFA NFA::star(NFA& nfa_) {
    NFA nfa;
    nfa.states = nfa_.states;

    nfa.initialState = nfa_.initialState;

    // Connect all the accepting states back to the initial state with epsilon.
    for(auto& state : nfa.states)
    {
        if(state.second->isAccepting)
        {
            state.second->addDestination(nfa.initialState, State::EPSILON);
        }
    }

    nfa.initialState->isAccepting = true;

    return nfa;
}

// Constructs a NFA from the given postfix expression.
NFA NFA::postfixToNfa(string regex_)
{
    stack<NFA> nfaStack;
    char c;

    while(regex_.size() > 0)
    {
        c = regex_[0];
        regex_ = regex_.substr(1);

        if('&' == c) {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            nfaStack.push(NFA::concatenate(nfa1, nfa2));
        }
        else if('|' == c) {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            nfaStack.push(NFA::unionOfNFAs(nfa1, nfa2));
        }
        else if('*' == c) {
            NFA nfa = nfaStack.top();
            nfaStack.pop();

            nfaStack.push(NFA::star(nfa));
        }
        else {
            NFA nfa = NFA::singleSymbol(c);
            nfaStack.push(nfa);
        }
    }

    return nfaStack.top();
}

// For debugging purposes.
void NFA::print()
{
    qDebug() << "Init: " << this->initialState->getId() << "\n";
    for(auto& kv : this->states)
    {
        qDebug() << kv.first << "\n";
        for(auto& dest : kv.second->destinations)
        {
            qDebug() << "  " << dest.first->getId() << " - " << dest.second << "\n";
        }
    }
}
