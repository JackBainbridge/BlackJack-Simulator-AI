#include "Renderer.h"
#include "Card.h"
#include <iostream>
#include <filesystem>

Renderer::Renderer() {
    // Create a green blackjack table
    table = cv::Mat(TABLE_HEIGHT, TABLE_WIDTH, CV_8UC3, cv::Scalar(0, 100, 0));
    loadAssets();
}

Renderer::~Renderer() {
    cv::destroyAllWindows();
}

std::string Renderer::getCardFilename(const Card& card) {
    // Convert Card object to filename like "ace_of_spades.png"
    std::string rankStr;
    std::string suitStr;

    // Map rank to string
    switch (card.rank) {
        case Rank::ACE: rankStr = "ace"; break;
        case Rank::TWO: rankStr = "2"; break;
        case Rank::THREE: rankStr = "3"; break;
        case Rank::FOUR: rankStr = "4"; break;
        case Rank::FIVE: rankStr = "5"; break;
        case Rank::SIX: rankStr = "6"; break;
        case Rank::SEVEN: rankStr = "7"; break;
        case Rank::EIGHT: rankStr = "8"; break;
        case Rank::NINE: rankStr = "9"; break;
        case Rank::TEN: rankStr = "10"; break;
        case Rank::JACK: rankStr = "jack"; break;
        case Rank::QUEEN: rankStr = "queen"; break;
        case Rank::KING: rankStr = "king"; break;
    }

    // Map suit to string
    switch (card.suit) {
        case Suit::SPADES: suitStr = "spades"; break;
        case Suit::HEARTS: suitStr = "hearts"; break;
        case Suit::DIAMONDS: suitStr = "diamonds"; break;
        case Suit::CLUBS: suitStr = "clubs"; break;
    }

    return "assets/cards/" + rankStr + "_of_" + suitStr + ".png";
}

void Renderer::loadAssets() {
    // Load all card images and resize them for display
    Rank ranks[] = {Rank::ACE, Rank::TWO, Rank::THREE, Rank::FOUR, Rank::FIVE,
                    Rank::SIX, Rank::SEVEN, Rank::EIGHT, Rank::NINE, Rank::TEN,
                    Rank::JACK, Rank::QUEEN, Rank::KING};
    Suit suits[] = {Suit::SPADES, Suit::HEARTS, Suit::DIAMONDS, Suit::CLUBS};

    for (Rank rank : ranks) {
        for (Suit suit : suits) {
            Card card(rank, suit);
            std::string filename = getCardFilename(card);
            
            cv::Mat img = cv::imread(filename);
            if (img.empty()) {
                std::cerr << "Warning: Could not load " << filename << std::endl;
                continue;
            }

            // Resize to standard card size for display
            cv::resize(img, img, cv::Size(CARD_WIDTH, CARD_HEIGHT));
            cardCache[filename] = img;
        }
    }

    std::cout << "Loaded " << cardCache.size() << " card images." << std::endl;
}

void Renderer::drawCard(int x, int y, const Card& card, bool hidden) {
    if (hidden) {
        // Draw the back of a card using red_joker.png
        cv::Mat cardBack = cv::imread("assets/cards/red_joker.png");
        if (cardBack.empty()) {
            // Fallback to blue rectangle if image not found
            cv::rectangle(table, cv::Point(x, y), 
                         cv::Point(x + CARD_WIDTH, y + CARD_HEIGHT), 
                         cv::Scalar(200, 100, 0), -1);
            cv::rectangle(table, cv::Point(x, y), 
                         cv::Point(x + CARD_WIDTH, y + CARD_HEIGHT), 
                         cv::Scalar(255, 200, 0), 2);
            cv::putText(table, "?", cv::Point(x + CARD_WIDTH/2 - 10, y + CARD_HEIGHT/2 + 10),
                       cv::FONT_HERSHEY_SIMPLEX, 1.5, cv::Scalar(255, 255, 255), 2);
        } else {
            // Resize to card size and draw
            cv::resize(cardBack, cardBack, cv::Size(CARD_WIDTH, CARD_HEIGHT));
            cv::Mat roi = table(cv::Rect(x, y, CARD_WIDTH, CARD_HEIGHT));
            cardBack.copyTo(roi);
            
            // Draw border around the card
            cv::rectangle(table, cv::Point(x, y), 
                         cv::Point(x + CARD_WIDTH, y + CARD_HEIGHT), 
                         cv::Scalar(255, 255, 255), 2);
        }
        return;
    }

    std::string filename = getCardFilename(card);
    
    // Check if card is in cache
    if (cardCache.find(filename) == cardCache.end()) {
        std::cerr << "Card not in cache: " << filename << std::endl;
        return;
    }

    cv::Mat cardImg = cardCache[filename];

    // Draw the card image onto the table
    cv::Mat roi = table(cv::Rect(x, y, CARD_WIDTH, CARD_HEIGHT));
    cardImg.copyTo(roi);

    // Draw a border around the card
    cv::rectangle(table, cv::Point(x, y), 
                 cv::Point(x + CARD_WIDTH, y + CARD_HEIGHT), 
                 cv::Scalar(255, 255, 255), 2);
}

void Renderer::displayState(const Hand& player, const Hand& dealer, std::string message, bool dealersTurn) {
    // 1. Refresh table to green
    table = cv::Mat(TABLE_HEIGHT, TABLE_WIDTH, CV_8UC3, cv::Scalar(0, 100, 0));

    // 2. Draw Dealer Section (Top)
    cv::putText(table, "DEALER", cv::Point(20, 30), 
               cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    
    int dealerX = 50;
    int dealerY = 60;
    
    // Draw dealer cards
    for (int i = 0; i < dealer.getSize(); i++) {
        bool isHidden = (i == 1 && !dealersTurn); // Hide second card only if not dealer's turn
        drawCard(dealerX + (i * (CARD_WIDTH + 10)), dealerY, dealer.getCard(i), isHidden);
    }

    // Dealer total - only show visible cards total
    int dealerVisibleTotal = dealer.getTotal();
    if (dealer.getSize() > 1 && !dealersTurn) {
        // Subtract the hidden card's value from the total
        dealerVisibleTotal -= dealer.getCard(1).getValue();
    }
    
    std::string dealerTotal = "Total: " + std::to_string(dealerVisibleTotal);
    cv::putText(table, dealerTotal, cv::Point(dealerX, dealerY + CARD_HEIGHT + 40),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // 3. Draw Player Section (Bottom)
    cv::putText(table, "PLAYER", cv::Point(20, TABLE_HEIGHT - 180), 
               cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

    int playerX = 50;
    int playerY = TABLE_HEIGHT - 150;

    // Draw player cards
    for (int i = 0; i < player.getSize(); i++) {
        drawCard(playerX + (i * (CARD_WIDTH + 10)), playerY, player.getCard(i), false);
    }

    // Player total
    std::string playerTotal = "Total: " + std::to_string(player.getTotal());
    cv::putText(table, playerTotal, cv::Point(playerX, playerY + CARD_HEIGHT + 40),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // 4. Display message in center
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    int thickness = 2;
    cv::Size textSize = cv::getTextSize(message, fontFace, fontScale, thickness, nullptr);
    
    int centerX = (TABLE_WIDTH - textSize.width) / 2;
    int centerY = TABLE_HEIGHT / 2;

    cv::putText(table, message, cv::Point(centerX, centerY),
               fontFace, fontScale, cv::Scalar(255, 255, 0), thickness);

    // 5. Show window
    cv::imshow("Blackjack AI", table);
    cv::waitKey(500); // 500ms delay for visualization
}

void Renderer::displayPrompt(const Hand& player, const Hand& dealer, const std::string& prompt, const std::string& result) {
    // Don't refresh the table - keep the current game state visible
    
    // Draw a semi-transparent overlay
    cv::Mat overlay = table.clone();
    cv::rectangle(overlay, cv::Point(0, 0), cv::Point(TABLE_WIDTH, TABLE_HEIGHT),
                 cv::Scalar(0, 0, 0), -1);
    cv::addWeighted(overlay, 0.3, table, 0.7, 0, table);

    // Display result above the prompt
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double resultScale = 1.5;
    int thickness = 2;
    cv::Size resultSize = cv::getTextSize(result, fontFace, resultScale, thickness, nullptr);
    
    int resultCenterX = (TABLE_WIDTH - resultSize.width) / 2;
    int resultCenterY = TABLE_HEIGHT / 2 - 60;
    cv::Scalar resultColor = (result == "YOU WIN!" ? cv::Scalar(0, 255, 0) : 
                          (result == "DEALER WINS!" ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 255)));

    // Draw result text
    cv::putText(table, result, cv::Point(resultCenterX, resultCenterY),
               fontFace, resultScale, resultColor, thickness);
    // Display prompt in center (lower than result)
    double promptScale = 1.2;
    cv::Size textSize = cv::getTextSize(prompt, fontFace, promptScale, thickness, nullptr);
    
    int centerX = (TABLE_WIDTH - textSize.width) / 2;
    int centerY = TABLE_HEIGHT / 2 + 20;

    // Draw a box around the prompt
    cv::rectangle(table, cv::Point(centerX - 20, centerY - 40), 
                 cv::Point(centerX + textSize.width + 20, centerY + 40),
                 cv::Scalar(100, 100, 100), -1);
    cv::rectangle(table, cv::Point(centerX - 20, centerY - 40), 
                 cv::Point(centerX + textSize.width + 20, centerY + 40),
                 cv::Scalar(255, 255, 255), 2);

    cv::putText(table, prompt, cv::Point(centerX, centerY + 10),
               fontFace, promptScale, cv::Scalar(255, 255, 0), thickness);

    // Show window and wait for key press
    cv::imshow("Blackjack AI", table);
    lastKeyPressed = cv::waitKey(0); // Wait indefinitely for key press
}

int Renderer::displayActionPrompt(const Hand& player, const Hand& dealer, const std::string& message) {
    // 1. Refresh table to green
    table = cv::Mat(TABLE_HEIGHT, TABLE_WIDTH, CV_8UC3, cv::Scalar(0, 100, 0));

    // 2. Draw Dealer Section (Top)
    cv::putText(table, "DEALER", cv::Point(20, 30), 
               cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);
    
    int dealerX = 50;
    int dealerY = 60;
    
    // Draw dealer cards
    for (int i = 0; i < dealer.getSize(); i++) {
        bool isHidden = (i == 1); // Hide second card
        drawCard(dealerX + (i * (CARD_WIDTH + 10)), dealerY, dealer.getCard(i), isHidden);
    }

    // Dealer total - only show visible cards total
    int dealerVisibleTotal = dealer.getTotal();
    if (dealer.getSize() > 1) {
        dealerVisibleTotal -= dealer.getCard(1).getValue();
    }
    
    std::string dealerTotal = "Total: " + std::to_string(dealerVisibleTotal);
    cv::putText(table, dealerTotal, cv::Point(dealerX, dealerY + CARD_HEIGHT + 40),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // 3. Draw Player Section (Bottom)
    cv::putText(table, "PLAYER", cv::Point(20, TABLE_HEIGHT - 220), 
               cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 255, 255), 2);

    int playerX = 50;
    int playerY = TABLE_HEIGHT - 190;

    // Draw player cards
    for (int i = 0; i < player.getSize(); i++) {
        drawCard(playerX + (i * (CARD_WIDTH + 10)), playerY, player.getCard(i), false);
    }

    // Player total
    std::string playerTotal = "Total: " + std::to_string(player.getTotal());
    cv::putText(table, playerTotal, cv::Point(playerX, playerY + CARD_HEIGHT + 40),
               cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(255, 255, 255), 2);

    // 4. Display action prompt in center with two options
    int fontFace = cv::FONT_HERSHEY_SIMPLEX;
    double fontScale = 1.0;
    int thickness = 2;
    
    // Main message
    cv::Size msgSize = cv::getTextSize(message, fontFace, fontScale, thickness, nullptr);
    int msgCenterX = (TABLE_WIDTH - msgSize.width) / 2;
    int msgCenterY = TABLE_HEIGHT / 2 - 40;
    cv::putText(table, message, cv::Point(msgCenterX, msgCenterY),
               fontFace, fontScale, cv::Scalar(255, 255, 0), thickness);

    // Hit option
    std::string hitText = "Press H to HIT";
    cv::Size hitSize = cv::getTextSize(hitText, fontFace, 0.8, thickness, nullptr);
    int hitCenterX = (TABLE_WIDTH - hitSize.width) / 2;
    int hitCenterY = TABLE_HEIGHT / 2 + 20;
    cv::rectangle(table, cv::Point(hitCenterX - 15, hitCenterY - 25),
                 cv::Point(hitCenterX + hitSize.width + 15, hitCenterY + 10),
                 cv::Scalar(0, 100, 200), -1);
    cv::rectangle(table, cv::Point(hitCenterX - 15, hitCenterY - 25),
                 cv::Point(hitCenterX + hitSize.width + 15, hitCenterY + 10),
                 cv::Scalar(255, 255, 255), 2);
    cv::putText(table, hitText, cv::Point(hitCenterX, hitCenterY),
               fontFace, 0.8, cv::Scalar(255, 255, 255), thickness);

    // Stand option
    std::string standText = "Press S to STAND";
    cv::Size standSize = cv::getTextSize(standText, fontFace, 0.8, thickness, nullptr);
    int standCenterX = (TABLE_WIDTH - standSize.width) / 2;
    int standCenterY = TABLE_HEIGHT / 2 + 70;
    cv::rectangle(table, cv::Point(standCenterX - 15, standCenterY - 25),
                 cv::Point(standCenterX + standSize.width + 15, standCenterY + 10),
                 cv::Scalar(150, 0, 150), -1);
    cv::rectangle(table, cv::Point(standCenterX - 15, standCenterY - 25),
                 cv::Point(standCenterX + standSize.width + 15, standCenterY + 10),
                 cv::Scalar(255, 255, 255), 2);
    cv::putText(table, standText, cv::Point(standCenterX, standCenterY),
               fontFace, 0.8, cv::Scalar(255, 255, 255), thickness);

    // 5. Show window and wait for key press
    cv::imshow("Blackjack AI", table);
    int key = cv::waitKey(0);
    
    // Return 1 for HIT (h), 0 for STAND (s)
    if (key == 'h' || key == 'H') {
        return 1;  // HIT
    } else if (key == 's' || key == 'S') {
        return 0;  // STAND
    }
    return 0;  // Default to STAND
}