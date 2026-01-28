#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include "Card.h"
#include "Deck.h"
#include "Hand.h"
#include <sqlite3.h>
#include <opencv2/opencv.hpp>

void playRound() {
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
    while (true) {
        std::cout << "Your total: " << playerHand.getTotal() << " | (h)it or (s)tand? ";
        char choice;
        std::cin >> choice;

        if (choice == 'h') {
            playerHand.addCard(deck.dealCard());
            std::cout << "You drew: " << playerHand.getCard(playerHand.getSize() - 1).toString() << std::endl;
            if (playerHand.isBust()) {
                std::cout << "BUST! Total: " << playerHand.getTotal() << ". Dealer wins." << std::endl;
                return;
            }
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

int main() {
    // Validate the environment setup
    std::cout << "--- Blackjack AI Initializing ---" << std::endl;

    // Test SQLite
    std::cout << "SQLite Version: " << sqlite3_libversion() << std::endl;

    // Test OpenCV (Creates a simple black window)
    cv::Mat table = cv::Mat::zeros(400, 600, CV_8UC3);
    cv::putText(table, "Blackjack Table", cv::Point(200, 200), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    
    std::cout << "Environment check passed!" << std::endl;

    // Start Game
    char playAgain = 'y';
    while (playAgain == 'y') {
        playRound();
        std::cout << "\nPlay another round? (y/n): ";
        std::cin >> playAgain;
    }

    std::cout << "Thanks for playing!" << std::endl;
    return EXIT_SUCCESS;
}