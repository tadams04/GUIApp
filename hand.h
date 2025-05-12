#ifndef HAND_H
#define HAND_H

#include "card.h"
#include "deck.h"
#include <array>
#include <QString>

// Represents a five card poker hand, has deck dealing, sorting and hand evaluation, and advanced spec

class Hand
{
public:
    Hand();

    // deal exactly 5 cards from deck into hand (-5, +5)
    void dealHand(Deck& deck);

    // Sorts cards in ascending order for card comparisons
    void sortValue();

    // Sort cards by group then by value, e.g. a pair of twos is sorted into the pairs group then all pairs sorted by value
    void sortGroup();

    // Swap
    void swapCards(const std::vector<int>& idx, Deck& deck);


    // Returns best poker category from the ten specified (High -> Low:
    // "ryfl", "stfl", "four", "full", "strt", "trio", "twop", "pair", "high"
    QString getBest() const;

    // Getter method for cards, use in GUi
    const std::array<Card,5>& cards() const;


private:
    std::array<Card,5> m_cards;

    // Internal evaluation helpers (not exposed in spec but used by getBest):
    bool isFlush() const;
    bool isStraight() const;
    std::pair<QString,int> classifyDuplicates() const;  // e.g. returns {"twop", highestPairValue}

};

#endif
