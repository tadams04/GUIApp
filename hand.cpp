#include "hand.h"
#include <algorithm>
#include <map>


Hand::Hand() = default;

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


// Group-aware sort: highest duplicate counts first, then higher value
void Hand::sortGroup()
{
    // 1) Build a frequency table for each card
    int freq[5];
    for (int i = 0; i < 5; ++i) {
        freq[i] = 1;
        for (int j = 0; j < 5; ++j) {
            if (j != i && m_cards[j].getValue() == m_cards[i].getValue()) {
                ++freq[i];
            }
        }
    }

    // 2) Selection-sort by (freq desc, value desc)
    for (int i = 0; i < 4; ++i) {
        int maxIndex = i;
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

// Swap
void Hand::swapCards(const std::vector<int>& idx, Deck& deck)
{
    // 1. Return the discards
    for (int i : idx)
        deck.insertCard(m_cards[i]);

    deck.shuffle();                 // randomise their future position

    // 2. Deal replacements into the SAME slots
    for (int i : idx)
        m_cards[i] = deck.dealCard();

    // 3. Keep helper invariants up-to-date
    sortValue();
    sortGroup();
}



QString Hand::getBest() const
{
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
        // Royal Flush if sequence ends in Ace (14) and starts at 10
        bool isRoyal = (high == 14 && low == 10);
        return isRoyal ? "ryfl" : "stfl";
    }

    // Look at duplicate structure
    auto dup = classifyDuplicates();
    if (!dup.first.isEmpty()) return dup.first;

    if (flush) return "flsh";
    if (straight) return "strt";
    return "high";
}


const std::array<Card,5>& Hand::cards() const {
    return m_cards;
}


bool Hand::isFlush() const
{
    // 1) Get the suit of the first card
    QString firstSuit = m_cards[0].getSuit();

    // 2) Compare every other card’s suit to that
    for (int i = 1; i < 5; ++i) {
        if (m_cards[i].getSuit() != firstSuit) {
            // As soon as one differs, it's not a flush
            return false;
        }
    }

    // If we never find a difference, all suits match → flush
    return true;
}


bool Hand::isStraight() const
{
    // Make a local copy of the five values and sort them
    int straightCheck[5];
    for (int i = 0; i < 5; ++i) straightCheck[i] = m_cards[i].getValue();
    std::sort(straightCheck, straightCheck + 5);

    // Normal consecutistraightChecke test (works for 10-J-Q-K-A too)
    bool regular = true;
    for (int i = 1; i < 5; ++i)
        if (straightCheck[i] != straightCheck[0] + i) regular = false;
    if (regular) return true;

    // Hard check for wraparound
    bool wraparound = straightCheck[0] == 2 && straightCheck[1] == 3 && straightCheck[2] == 4 && straightCheck[3] == 5 && straightCheck[4] == 14;
    return wraparound;
}



std::pair<QString,int> Hand::classifyDuplicates() const
{
    // Build frequency map  value → count
    std::map<int,int> freq;
    for (const Card& c : m_cards)
        ++freq[c.getValue()];

    // Gather stats
    int numPairs   = 0;
    int threeKind  = 0;
    int fourKind   = 0;
    int highPair   = 0;     // highest value that forms a pair
    int tripValue  = 0;     // value that forms the trip
    int quadValue  = 0;     // value that forms the quad

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
        // No need for an `else` since the default case does nothing
    }

    // Decide category
    if (fourKind) return {"four", quadValue = fourKind};
    if (threeKind && numPairs == 1) return {"full", tripValue = threeKind};
    if (threeKind) return {"trio", tripValue = threeKind};
    if (numPairs == 2) return {"twop", highPair};
    if (numPairs == 1) return {"pair", highPair};
    return {"", 0};
}
