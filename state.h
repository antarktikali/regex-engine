#include <unordered_map>
using namespace std;

#ifndef STATE_H
#define	STATE_H

class State
{
public:
    State(unordered_map<int, State*>*);
    bool isAccepting;
    int getId();
    void addDestination(State*, char);
    unordered_map<State*, char> destinations;
    static const char EPSILON = '$';
private:
    int id;
    static int idCounter;
};

#endif	/* STATE_H */
