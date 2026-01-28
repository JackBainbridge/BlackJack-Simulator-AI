#include <iostream>
#include <sqlite3.h>
#include <opencv2/opencv.hpp>

int main() {
    std::cout << "--- Blackjack AI Initializing ---" << std::endl;

    // Test SQLite
    std::cout << "SQLite Version: " << sqlite3_libversion() << std::endl;

    // Test OpenCV (Creates a simple black window)
    cv::Mat table = cv::Mat::zeros(400, 600, CV_8UC3);
    cv::putText(table, "Blackjack Table", cv::Point(200, 200), 
                cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
    
    std::cout << "Environment check passed!" << std::endl;
    return 0;
}