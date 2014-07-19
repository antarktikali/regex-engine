#include <unordered_map>
#include <string>
#include "state.h"
using namespace std;

#ifndef NFA_H
#define	NFA_H

class NFA
{

public:
    NFA();
    unordered_map<int, State*> states;

    bool accepts(string);
    unordered_map<int, State*> epsilonClosure(State*, unordered_map<int, State*>*);
    void print();

    static NFA singleSymbol(char c);
    static NFA unionOfNFAs(NFA&, NFA&);
    static NFA concatenate(NFA&, NFA&);
    static NFA star(NFA&);
    static NFA postfixToNfa(string);
private:
    bool hasAcceptingStates(unordered_map<int, State*>*);

    State* initialState;
};

#endif	/* NFA_H */
