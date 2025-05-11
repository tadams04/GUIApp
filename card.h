#ifndef CARD_H
#define CARD_H

#include <QString>
#include <array>

// Type‑safe enumeration of the four suits
enum class Suit {
    Hearts = 0,
    Diamonds,
    Clubs,
    Spades
};

/**
 * Simple value‑object that represents one playing card.
 *
 *  • m_value is 1–13 where 1 = Ace, 11 = Jack, 12 = Queen, 13 = King.
 *  • The deck class will treat Ace as 14 when needed (for high‑Ace straights).
 */
class Card
{
public:
    explicit Card(int value = 0, Suit suit = Suit::Hearts);

    int getValue() const;      ///< 1–13 (Ace low = 1)
    QString getSuit()  const;      ///< Unicode symbol ♥ ♦ ♣ ♠
    QString getName()  const;      ///< e.g. "10♠", "Q♦", "A♥"
    int getNumber() const;     ///< unique 0‑51 index  (suit*13 + value‑1)

private:
    int  m_value;  // 1‑13
    Suit m_suit;   // enum
};

#endif // CARD_H
