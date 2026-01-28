#include <string>
#include "Card.h"

Card::Card(Rank r, Suit s) : rank(r), suit(s) {}

int Card::getValue() const {
    if (rank >= Rank::JACK) return 10;
    if (rank == Rank::ACE) return 11; // Logic for 1 vs 11 is handled in the Hand class
    return static_cast<int>(rank);
}

std::string Card::toString() const {
    std::string ranks[] = {"", "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    std::string suits[] = {"Spades", "Hearts", "Diamonds", "Clubs"};
    return ranks[static_cast<int>(rank)] + " of " + suits[static_cast<int>(suit)];
}