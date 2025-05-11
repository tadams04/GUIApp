#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <random>

/**
 * Represents a standard 52-card deck that can be
 * created, shuffled, and dealt from.
 */
class Deck
{
public:
    Deck();

    /**
     * Populate m_cards with all 52 unique Card objects.
     */
    void createDeck();

    /**
     * Randomly shuffle the deck using a Mersenne Twister RNG.
     */
    void shuffle();

    /**
     * Deal (pop) one card from the end; assumes non-empty deck.
     * @return the dealt Card.
     */
    Card dealCard();

    /**
     * Number of cards left in the deck.
     * @return size of the deck.
     */
    size_t cardsRemaining() const;

    /**
     * Reset to a full, shuffled deck.
     */
    void reset();

private:
    std::vector<Card> m_cards;
    std::mt19937 m_rng;  // Random Number gen
};

#endif // DECK_H
