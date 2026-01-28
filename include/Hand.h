#ifndef HAND_H
#define HAND_H

#include <vector>
#include "Card.h"

class Hand {
private:
    std::vector<Card> cards;
public:
    Hand();
    Card getCard(int index) const;
    void addCard(const Card& card);
    int getTotal() const;
    bool isBust() const;
    int getSize() const;
};
#endif