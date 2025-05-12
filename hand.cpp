#include "hand.h"
#include <algorithm>
#include <map>


Hand::Hand() = default;

// Draws fve new cards from deck
void Hand::dealHand(Deck& deck) {
    for(int i = 0; i < 5; i++) {
        m_cards[i] = deck.dealCard();
    }
}


void Hand::sortValue() {
        // For each position i from 0 to 3
        for (int i = 0; i < 4; ++i) {
            // Find the index of the smallest value in the range [i, 4]
            int minIndex = i;
            for (int j = i + 1; j < 5; ++j) {
                if (m_cards[j].getValue() < m_cards[minIndex].getValue()) {
                    minIndex = j;
                }
            }
            // Swap the card at i with the smallest found
            if (minIndex != i) {
                std::swap(m_cards[i], m_cards[minIndex]);
            }
        }
    }


// Sorts by how many duplicates each card has then by card value
void Hand::sortGroup() {
    // Builds a sort of frequency table, where freq[i] is equal to number of cards in hand matching m_card[i].getValue()
    int freq[5];
    for (int i = 0; i < 5; ++i) {
        freq[i] = 1; // Inlucdes card itself
        for (int j = 0; j < 5; ++j) {
            // If different index but same value then dupe
            if (j != i && m_cards[j].getValue() == m_cards[i].getValue()) {
                ++freq[i];
            }
        }
    }

    // Selection sort over the 5 cards
    for (int i = 0; i < 4; ++i) {
        int maxIndex = i;
        // Compares duplicate counts
        for (int j = i + 1; j < 5; ++j) {
            bool higherFreq = freq[j] > freq[maxIndex];
            bool sameFreqButHigherValue =
                (freq[j] == freq[maxIndex] &&
                 m_cards[j].getValue() > m_cards[maxIndex].getValue());
            if (higherFreq || sameFreqButHigherValue) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            std::swap(m_cards[i], m_cards[maxIndex]);
            std::swap(freq[i],   freq[maxIndex]);
        }
    }
}

// Replaces selected indices with fresh cards, keeps original order to stop weird hand graphic change
void Hand::swapCards(const std::vector<int>& idx, Deck& deck) {
    // Returns the cards player wants to swap to deck
    for (int i : idx) {
        deck.insertCard(m_cards[i]);
    }

    deck.shuffle(); // randomise deck

    // Takes swap replcaements and puts in same spots
    for (int i : idx)
        m_cards[i] = deck.dealCard();

    // Caused index movement on swap
    // sortValue();
    // sortGroup();
}


// Returns category code
QString Hand::getBest() const {
    bool flush = isFlush();
    bool straight = isStraight();

    // For straight-flush branch we need the highest card value
    int high = 0;
    int low = 15;
    for (const Card& c : m_cards) {
        high = std::max(high, c.getValue());
        low  = std::min(low,  c.getValue());
    }

    if (straight && flush)
    {
        // Royal Flush if sequence ends in ace and starts with 10
        bool isRoyal = (high == 14 && low == 10);
        if (isRoyal) {
            return "ryfl";
        }
        return "stfl";
    }

    auto dup = classifyDuplicates();
    if (!dup.first.isEmpty()) {
        return dup.first;
    }
    if (flush) {
        return "flsh";
    }
    if (straight) {
        return "strt";
    }
    return "high";
}


const std::array<Card,5>& Hand::cards() const {
    return m_cards;
}


bool Hand::isFlush() const {
    // Gets first cards suit for comparison
    QString firstSuit = m_cards[0].getSuit();

    // All cards compared to that suit
    for (int i = 1; i < 5; ++i) {
        if (m_cards[i].getSuit() != firstSuit) {
            // With one card differeing we know it is not a flush
            return false;
        }
    }
    // Flush
    return true;
}


bool Hand::isStraight() const {
    // Make a local copy of the five values and sort them
    int straightCheck[5];
    for (int i = 0; i < 5; ++i) {
        straightCheck[i] = m_cards[i].getValue();
    }
    std::sort(straightCheck, straightCheck + 5);

    bool regular = true;
    for (int i = 1; i < 5; ++i)
        if (straightCheck[i] != straightCheck[0] + i) regular = false;
    if (regular) return true;

    // Hard check for wraparound, may be better way but oh well
    bool wraparound = straightCheck[0] == 2 && straightCheck[1] == 3 && straightCheck[2] == 4 && straightCheck[3] == 5 && straightCheck[4] == 14;
    return wraparound;
}


// Helper for duplciates
std::pair<QString,int> Hand::classifyDuplicates() const {
    std::map<int,int> freq;
    for (const Card& c : m_cards)
        ++freq[c.getValue()];

    // Gather stats
    int numPairs = 0;
    int threeKind = 0;
    int fourKind = 0;
    int highPair = 0; // highest value that forms a pair
    int tripValue = 0; // value that forms the trip
    int quadValue = 0; // value that forms the quad

    for (auto [val, count] : freq)
    {
        if (count == 4)
        {
            fourKind = val;
        }
        else if (count == 3)
        {
            threeKind = val;
        }
        else if (count == 2)
        {
            ++numPairs;
            highPair = std::max(highPair, val);
        }
    }

    // Decide category
    if (fourKind) {
        quadValue = fourKind;
        return {"four", quadValue};
    }

    if (threeKind) {
        tripValue = threeKind;
        if (numPairs == 1) {
            return {"full", tripValue};
        }
        return {"trio", tripValue};
    }

    if (numPairs == 2) {
        return {"twop", highPair};
    }

    if (numPairs == 1) {
        return {"pair", highPair};
    }

    return {"", 0};
}
