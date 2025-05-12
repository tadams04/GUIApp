#ifndef CARD_H
#define CARD_H

#include <QString>
#include <array>

// Suit enumeration: keep this order ↴
enum class Suit {
    Hearts = 0,
    Diamonds,
    Clubs,
    Spades
};

/**
 * Simple value-object that represents one playing card.
 *
 *  • m_value is 1–13 where 1 = Ace, 11 = Jack, 12 = Queen, 13 = King.
 *  • getName() now returns strings such as  "10_of_diamonds"
 */
class Card
{
public:
    explicit Card(int value = 0, Suit suit = Suit::Hearts);

    int     getValue() const;   ///< 1-13  (Ace low = 1)
    QString getSuit()  const;   ///< "hearts", "diamonds", …
    QString getName()  const;   ///< "queen_of_spades", "7_of_clubs"
    int     getNumber() const;  ///< suit*13 + value-1   (0-51)

private:
    int  m_value;   // 1-13
    Suit m_suit;    // enum
};

#endif // CARD_H
