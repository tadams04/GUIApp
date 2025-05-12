#include "deck.h"
#include <algorithm>
#include <chrono>

Deck::Deck()
{
    // Seed RNG clcck
    m_rng.seed(
        static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    createDeck();
}

// Builds the deck, 4 suites, 13 cards per suite
void Deck::createDeck() {
    m_cards.clear();
    for (int s = 0; s < 4; ++s) {
        for (int v = 1; v <= 13; ++v) {
            m_cards.emplace_back(v, static_cast<Suit>(s));
        }
    }
}

void Deck::shuffle() {
    std::shuffle(m_cards.begin(), m_cards.end(), m_rng);
}

// Pops card from back and returns it
Card Deck::dealCard() {
    Card c = m_cards.back();
    m_cards.pop_back();
    return c;
}

size_t Deck::cardsRemaining() const {
    return m_cards.size();
}

// Swap
void Deck::insertCard(const Card& c) {
    m_cards.push_back(c);
}

void Deck::reset() {
    createDeck();
    shuffle();
}
