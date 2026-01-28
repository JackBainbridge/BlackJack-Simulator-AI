#ifndef QLEARNER_H
#define QLEARNER_H

#include <map>
#include <string>
#include <tuple>
#include "Hand.h"
#include <sqlite3.h>


struct State {
    int pTotal;
    int dCard;
    bool hasAce;

    bool operator<(const State& other) const {
        return std::tie(pTotal, dCard, hasAce) < std::tie(other.pTotal, other.dCard, other.hasAce);
    }
};

class QLearner {
public:
    std::map<State, double[2]> qTable; // 0: Stand, 1: Hit
    
    double alpha = 0.1;    // Learning rate
    double gamma = 0.9;    // Discount factor
    double epsilon = 0.2;  // Exploration rate (20% of time try random)

    int decide(State s, bool training = true);
    void update(State s, int action, double reward, State nextS, bool isDone);
    void saveToDatabase(const std::string& filename);
    void loadFromDatabase(const std::string& filename);
};

#endif