#include <iostream>
#include "Trainer.h"
#include "QLearner.h"
#include "Deck.h"
#include "Hand.h"

void runSilentTrainer(QLearner& ai, int totalHands) {
    std::cout << "Training AI for " << totalHands << " hands..." << std::endl;

    for (int i = 0; i < totalHands; ++i) {
        Deck deck;
        deck.shuffle();
        Hand player, dealer;

        // Initial Deal
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());
        player.addCard(deck.dealCard());
        dealer.addCard(deck.dealCard());

        State currentState = {player.getTotal(), dealer.getCard(0).getValue(), false};
        
        // Player's Turn
        while (!player.isBust()) {
            int action = ai.decide(currentState, true);
            if (action == 0) break; // Stand

            player.addCard(deck.dealCard());
            State nextState = {player.getTotal(), dealer.getCard(0).getValue(), false};
            
            if (player.isBust()) {
                ai.update(currentState, 1, -1.0, nextState, true);
            } else {
                ai.update(currentState, 1, 0, nextState, false);
            }
            currentState = nextState;
        }

        // Dealer's Turn & Final Reward
        if (!player.isBust()) {
            while (dealer.getTotal() < 17) dealer.addCard(deck.dealCard());

            double reward = 0;
            if (dealer.isBust() || player.getTotal() > dealer.getTotal()) reward = 1.0;
            else if (player.getTotal() < dealer.getTotal()) reward = -1.0;

            ai.update(currentState, 0, reward, currentState, true);
        }
    }
    std::cout << "Training complete. Q-Table size: " << ai.qTable.size() << " states." << std::endl;
}