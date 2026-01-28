#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

enum class Suit { SPADES, HEARTS, DIAMONDS, CLUBS };
enum class Rank { ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING };

class Card {
public:
    Suit suit;
    Rank rank;
    Card(Rank r = Rank::ACE, Suit s = Suit::SPADES);
    int getValue() const;
    std::string toString() const;
};

#endif