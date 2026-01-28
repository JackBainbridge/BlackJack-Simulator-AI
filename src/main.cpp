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
#include "Renderer.h"
#include <opencv2/opencv.hpp>

void playRound(QLearner& ai, int playMode, Renderer* gui) {
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

    // Check for player blackjack (21 with first 2 cards)
    if (playerHand.getSize() == 2 && playerHand.getTotal() == 21) {
        std::cout << "BLACKJACK! Player wins!" << std::endl;
        
        if (gui) {
            gui->displayState(playerHand, dealerHand, "BLACKJACK!", false);
            gui->displayPrompt(playerHand, dealerHand, "Play another round? (Y/N)", "BLACKJACK!");
        }
        return;
    }

    // Player Turn
    while (!playerHand.isBust()) {
        // Update Visuals if GUI is active
        if (gui) {
            gui->displayState(playerHand, dealerHand, "Player's Turn", false);
        }

        int action;
        std::cout << "Player Cards: " << playerHand.getCard(0).toString() << ", " << playerHand.getCard(1).toString() << std::endl;
        std::cout << "Player Total: " << playerHand.getTotal() << std::endl;

        if (playMode == 0) { // MANUAL MODE
            if (gui) {
                action = gui->displayActionPrompt(playerHand, dealerHand, "Your Turn!");
                std::cout << (action == 1 ? "Player chose: HIT" : "Player chose: STAND") << std::endl;
            } else {
                std::cout << "(h)it or (s)tand? ";
                char choice; 
                std::cin >> choice;
                action = (choice == 'h' || choice == 'H') ? 1 : 0;
            }
        } 
        else { // AI MODE
            State s = { playerHand.getTotal(), dealerHand.getCard(0).getValue(), false };
            action = ai.decide(s, false); // No randomness in play mode
            std::cout << "AI decides to: " << (action == 1 ? "HIT" : "STAND") << std::endl;
        }

        if (action == 1) {
            playerHand.addCard(deck.dealCard());
            std::cout << "Drew: " << playerHand.getCard(playerHand.getSize() - 1).toString() << std::endl;
        } else {
            break;
        }
    }

    // Check for player bust
    if (playerHand.isBust()) {
        std::cout << "Player busts with total: " << playerHand.getTotal() << std::endl;
        
        if (gui) {
            gui->displayState(playerHand, dealerHand, "Player BUSTS!", false);
            gui->displayPrompt(playerHand, dealerHand, "Play another round? (Y/N)", "DEALER WINS!");
        }
        return;
    }

    // Dealer Turn (Must hit until 17)
    std::cout << "\nDealer reveals: " << dealerHand.getCard(1).toString() << std::endl;
    while (dealerHand.getTotal() < 17) {
        dealerHand.addCard(deck.dealCard());
        std::cout << "Dealer hits: " << dealerHand.getCard(dealerHand.getSize() - 1).toString() << std::endl;

        if (gui) {
            gui->displayState(playerHand, dealerHand, "Dealer's Turn", true);
        }
    }

    // Show final dealer hand
    if (gui) {
        gui->displayState(playerHand, dealerHand, "Dealer's Turn", true);
    }

    // Determine Winner
    int pTotal = playerHand.getTotal();
    int dTotal = dealerHand.getTotal();

    std::cout << "\nFinal Scores - Player: " << pTotal << " | Dealer: " << dTotal << std::endl;

    std::string result;
    if (dTotal > 21 || (pTotal > dTotal && pTotal <= 21)) {
        result = "YOU WIN!";
        std::cout << result << std::endl;
    } else if (pTotal < dTotal) {
        result = "DEALER WINS!";
        std::cout << result << std::endl;
    } else {
        result = "IT'S A PUSH (TIE)!";
        std::cout << result << std::endl;
    }
    
    std::cout << "Round finished." << std::endl;

    // Display result with prompt
    if (gui) {
        gui->displayPrompt(playerHand, dealerHand, "Play another round? (Y/N)", result);
    }
}

int main(int argc, char* argv[]) {
    std::cout << "--- Blackjack AI Initializing ---" << std::endl;
    std::cout << "SQLite Version: " << sqlite3_libversion() << std::endl;
    std::cout << "Environment check passed!" << std::endl;

    QLearner myAI;
    std::string dbFile = "blackjack_brain.db";

    // Parse command line arguments
    int trainMode = 0;  // 0 = Train, 1 = Load
    int playMode = 0;   // 0 = Manual, 1 = AI
    int guiMode = 0;    // 0 = Console, 1 = GUI

    if (argc > 1) trainMode = std::stoi(argv[1]);
    if (argc > 2) playMode = std::stoi(argv[2]);
    if (argc > 3) guiMode = std::stoi(argv[3]);

    // Handle Training/Loading
    if (trainMode == 0) {
        std::cout << "--- [MODE: TRAINING AI] ---" << std::endl;
        runSilentTrainer(myAI, 250000); 
        myAI.saveToDatabase(dbFile);
    } else {
        myAI.loadFromDatabase(dbFile);
        if (myAI.qTable.empty()) {
            std::cout << "--- [MODE: DATABASE EMPTY - TRAINING] ---" << std::endl;
            runSilentTrainer(myAI, 250000);
            myAI.saveToDatabase(dbFile);
        }
    }

    // Initialize GUI if requested
    Renderer* gui = nullptr;
    if (guiMode == 1) {
        gui = new Renderer(); 
    }

    // Game Loop
    char playAgain = 'y';
    while (playAgain == 'y') {
        playRound(myAI, playMode, gui);
        
        if (gui) {
            int key = gui->getKeyPressed();
            playAgain = (key == 'y' || key == 'Y') ? 'y' : 'n';
            gui->resetKeyPressed();
        } else {
            std::cout << "\nPlay another round? (y/n): ";
            std::cin >> playAgain;
        }
    }

    if (gui) delete gui;
    std::cout << "Thanks for playing!" << std::endl;
    
    return EXIT_SUCCESS;
}