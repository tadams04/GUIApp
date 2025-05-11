#include "deck.h"
#include <algorithm>
#include <chrono>

Deck::Deck()
{
    // Seed RNG with high-resolution clock
    m_rng.seed(
        static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    createDeck();
}

void Deck::createDeck()
{
    m_cards.clear();
    for (int s = 0; s < 4; ++s) {
        for (int v = 1; v <= 13; ++v) {
            m_cards.emplace_back(v, static_cast<Suit>(s));
        }
    }
}

void Deck::shuffle()
{
    std::shuffle(m_cards.begin(), m_cards.end(), m_rng);
}

Card Deck::dealCard()
{
    Card c = m_cards.back();
    m_cards.pop_back();
    return c;
}

size_t Deck::cardsRemaining() const
{
    return m_cards.size();
}

void Deck::reset()
{
    createDeck();
    shuffle();
}
