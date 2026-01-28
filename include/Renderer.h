#ifndef RENDERER_H
#define RENDERER_H

#include <map>
#include <string>
#include <opencv2/opencv.hpp>
#include "Hand.h"

class Renderer {
private:
    cv::Mat table;
    std::map<std::string, cv::Mat> cardCache; // Stores resized card images
    int CARD_WIDTH = 60;
    int CARD_HEIGHT = 90;
    int TABLE_WIDTH = 700;
    int TABLE_HEIGHT = 600;
    bool windowClosed = false;
    int lastKeyPressed = -1;

    // Helper function to build card filename from Card object
    std::string getCardFilename(const Card& card);
    
    // Helper to draw a card at a specific position
    void drawCard(int x, int y, const Card& card, bool hidden = false);

public:
    Renderer();
    ~Renderer();
    void loadAssets(); // Load all card PNG files
    void displayState(const Hand& player, const Hand& dealer, std::string message, bool dealersTurn = false);    
    void displayPrompt(const Hand& player, const Hand& dealer, const std::string& prompt, const std::string& result = "");
    int displayActionPrompt(const Hand& player, const Hand& dealer, const std::string& message);
    int getKeyPressed() const { return lastKeyPressed; }
    void resetKeyPressed() { lastKeyPressed = -1; }
    void resizeWindow(int width, int height);
    bool isWindowClosed() const { return windowClosed; }
    void closeWindow() { windowClosed = true; cv::destroyAllWindows(); }
};

#endif