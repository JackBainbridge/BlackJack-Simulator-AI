#include <algorithm>
#include <random>
#include <sqlite3.h>
#include <iostream>
#include "QLearner.h"

void QLearner::saveToDatabase(const std::string& filename) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) return;

    // 1. Create Table and Clear old data
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS QTable (pTotal INT, dCard INT, hasAce INT, standQ REAL, hitQ REAL);", 0, 0, 0);
    sqlite3_exec(db, "DELETE FROM QTable;", 0, 0, 0);

    // 2. Use a Transaction for massive speed boost
    sqlite3_exec(db, "BEGIN TRANSACTION;", 0, 0, 0);

    const char* sql = "INSERT INTO QTable VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    for (auto const& [state, values] : qTable) {
        sqlite3_bind_int(stmt, 1, state.pTotal);
        sqlite3_bind_int(stmt, 2, state.dCard);
        sqlite3_bind_int(stmt, 3, state.hasAce ? 1 : 0);
        sqlite3_bind_double(stmt, 4, values[0]); // Stand
        sqlite3_bind_double(stmt, 5, values[1]); // Hit
        
        sqlite3_step(stmt);
        sqlite3_reset(stmt);
    }

    sqlite3_finalize(stmt);
    sqlite3_exec(db, "END TRANSACTION;", 0, 0, 0);
    sqlite3_close(db);
    std::cout << "AI knowledge saved to " << filename << std::endl;
}

void QLearner::loadFromDatabase(const std::string& filename) {
    sqlite3* db;
    if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) return;

    const char* sql = "SELECT * FROM QTable;";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK) {
        qTable.clear();
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            State s;
            s.pTotal = sqlite3_column_int(stmt, 0);
            s.dCard = sqlite3_column_int(stmt, 1);
            s.hasAce = sqlite3_column_int(stmt, 2) == 1;
            
            qTable[s][0] = sqlite3_column_double(stmt, 3);
            qTable[s][1] = sqlite3_column_double(stmt, 4);
        }
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    std::cout << "AI knowledge loaded. States known: " << qTable.size() << std::endl;
}

int QLearner::decide(State s, bool training) {
    // Epsilon-greedy: Exploration
    if (training && ((double)rand() / RAND_MAX < epsilon)) {
        return rand() % 2; 
    }
    
    // Exploitation: Choose the best-known move
    return (qTable[s][1] > qTable[s][0]) ? 1 : 0;
}

void QLearner::update(State s, int action, double reward, State nextS, bool isDone) {
    double maxNextQ = isDone ? 0 : std::max(qTable[nextS][0], qTable[nextS][1]);
    
    // The Bellman Equation: 
    // NewQ = OldQ + LearningRate * (Reward + Discount * MaxFutureQ - OldQ)
    qTable[s][action] += alpha * (reward + gamma * maxNextQ - qTable[s][action]);
}