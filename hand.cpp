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


QString Hand::getBest() const {
    return QString("TODO");
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
    // Assumes sortValue() has been called
    for (int i = 1; i < 5; ++i) {
        if (m_cards[i].getValue() != m_cards[i-1].getValue()+1) return false;
    }
    return true;
}


std::pair<QString,int> Hand::classifyDuplicates() const
{
    // Build frequency map and find highest grouping
    std::map<int,int> freq;
    for (auto& c : m_cards) freq[c.getValue()]++;
    // TODO: analyze freq to determine category and key value
    return {"pair", 0}; // stub
}
