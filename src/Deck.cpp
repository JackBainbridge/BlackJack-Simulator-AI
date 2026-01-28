#include <vector>
#include <stdexcept>
#include <algorithm>
#include <random>
#include <chrono>
#include "Card.h"
#include "Deck.h"

Deck::Deck() {
    for (int s = static_cast<int>(Suit::SPADES); s <= static_cast<int>(Suit::CLUBS); ++s) {
        for (int r = static_cast<int>(Rank::ACE); r <= static_cast<int>(Rank::KING); ++r) {
            cards.emplace_back(static_cast<Rank>(r), static_cast<Suit>(s));
        }
    }
}

void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(seed));
}

Card Deck::dealCard() {
    if (cards.empty()) {
        // In a real game, you'd re-shuffle a new deck here
        throw std::out_of_range("Deck is empty!");
    }
    Card c = cards.back();
    cards.pop_back();
    return c;
}