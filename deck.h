#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <random>


// A 52 card deck that can shuffle, deal and accept returned cards and output for swaps
class Deck {
public:
    Deck();

    // Creates 52 cards, 4 suites, 13 unique per suite
    void createDeck();


    void shuffle();

    // Pop card from back
    Card dealCard();


    size_t cardsRemaining() const;

    // Push returned card
    void insertCard(const Card& c);

    // Recreates deck and reshuffles
    void reset();

private:
    std::vector<Card> m_cards;
    std::mt19937 m_rng;
};

#endif
