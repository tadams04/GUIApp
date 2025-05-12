#ifndef CARD_H
#define CARD_H

#include <QString>

// Suit enumeration, order used in getNumeer
enum class Suit {
    Hearts = 0, Diamonds, Clubs, Spades
};

// Represents any one card in the standard 52 card deck
class Card {
public:
    Card(int value = 0, Suit suit = Suit::Hearts);

    int getValue() const;   // Value 1 to 13
    QString getSuit() const;   //Card suit
    QString getName() const;   // Name, used for accessing correct images of files
    int getNumber() const;

private:
    int  m_value;   // 1-13
    Suit m_suit;    // enum
};

#endif
