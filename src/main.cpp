#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <sqlite3.h>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include "QLearner.h"
#include "Trainer.h"
#include <opencv2/opencv.hpp>

void playRound(QLearner& ai, int playMode) {
    // Placeholder for game logic
    std::cout << "Starting a new round of Blackjack..." << std::endl;

    Deck deck;
    deck.shuffle();
    
    Hand playerHand;
    Hand dealerHand;

    // Initial Deal
    playerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());
    playerHand.addCard(deck.dealCard());
    dealerHand.addCard(deck.dealCard());

    std::cout << "\n--- New Round ---" << std::endl;
    std::cout << "Dealer shows: " << dealerHand.getCard(0).toString() << " [Hidden]" << std::endl;

    // Player Turn
    while (!playerHand.isBust()) {
        int action;
        std::cout << "Player Cards: " << playerHand.getCard(0).toString() << ", " << playerHand.getCard(1).toString() << std::endl;
        std::cout << "Player Total: " << playerHand.getTotal() << std::endl;

        if (playMode == 0) { // MANUAL MODE
            std::cout << "(h)it or (s)tand? ";
            char choice; std::cin >> choice;
            action = (choice == 'h') ? 1 : 0;
        } 
        else { // AI MODE
            State s = { playerHand.getTotal(), dealerHand.getCard(0).getValue(), false };
            action = ai.decide(s, false); // No randomness
            std::cout << "AI decides to: " << (action == 1 ? "HIT" : "STAND") << std::endl;
        }

        if (action == 1) {
            playerHand.addCard(deck.dealCard());
            std::cout << "Drew: " << playerHand.getCard(playerHand.getSize() - 1).toString() << std::endl;
        } else {
            break;
        }
    }

    // Dealer Turn (Must hit until 17)
    std::cout << "\nDealer reveals: " << dealerHand.getCard(1).toString() << std::endl;
    while (dealerHand.getTotal() < 17) {
        dealerHand.addCard(deck.dealCard());
        std::cout << "Dealer hits: " << dealerHand.getCard(dealerHand.getSize() - 1).toString() << std::endl;
    }

    // Determine Winner
    int pTotal = playerHand.getTotal();
    int dTotal = dealerHand.getTotal();

    std::cout << "\nFinal Scores - Player: " << pTotal << " | Dealer: " << dTotal << std::endl;

    if (dTotal > 21 || pTotal > dTotal) {
        std::cout << "YOU WIN!" << std::endl;
    } else if (pTotal < dTotal) {
        std::cout << "DEALER WINS!" << std::endl;
    } else {
        std::cout << "IT'S A PUSH (TIE)!" << std::endl;
    }

    // Game logic would go here
    std::cout << "Round finished." << std::endl;
}

int main(int argc, char* argv[]) {
    // Validate the environment setup
    std::cout << "--- Blackjack AI Initializing ---" << std::endl;

    // Test SQLite
    std::cout << "SQLite Version: " << sqlite3_libversion() << std::endl;

    // Test OpenCV (Creates a simple black window)
    cv::Mat table = cv::Mat::zeros(400, 600, CV_8UC3);
    cv::putText(table, "Blackjack Table", cv::Point(200, 200), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    
    std::cout << "Environment check passed!" << std::endl;
    // ----------------------------------------------------------------------------
    QLearner myAI;
    std::string dbFile = "blackjack_brain.db";

    // Defaults
    int trainMode = 0; // Train
    int playMode = 0;  // Manual

    if (argc > 1) trainMode = std::stoi(argv[1]);
    if (argc > 2) playMode = std::stoi(argv[2]);

    // Handle Training/Loading - if empty DB, train first 250k hands
    if (trainMode == 0) {
        std::cout << "--- [MODE: FORCE TRAINING] ---" << std::endl;
        runSilentTrainer(myAI, 250000); 
        myAI.saveToDatabase(dbFile);
    } else {
        myAI.loadFromDatabase(dbFile);
        if (myAI.qTable.empty()) {
            runSilentTrainer(myAI, 250000);
            myAI.saveToDatabase(dbFile);
        }
    }

    // Start Game Loop
    char playAgain = 'y';
    while (playAgain == 'y') {
        // Pass the playMode to the game loop
        playRound(myAI, playMode); 
        std::cout << "\nPlay another round? (y/n): ";
        std::cin >> playAgain;
    }

    std::cout << "Thanks for playing!" << std::endl;
    return EXIT_SUCCESS;
}