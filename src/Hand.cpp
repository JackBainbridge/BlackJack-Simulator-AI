#include <vector>
#include "Card.h"
#include "Hand.h"

Hand::Hand() {}

void Hand::addCard(const Card& c) {
    cards.push_back(c);
}

Card Hand::getCard(int index) const {
    return cards.at(index); 
}

int Hand::getTotal() const {
    int total = 0;
    int aces = 0;

    for (const auto& card : cards) {
        total += card.getValue();
        if (card.rank == Rank::ACE) aces++;
    }

    // Adjust for Aces: if we bust, turn 11s into 1s
    while (total > 21 && aces > 0) {
        total -= 10;
        aces--;
    }
    return total;
}

bool Hand::isBust() const { return getTotal() > 21; }

int Hand::getSize() const { return cards.size(); }