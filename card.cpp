#include "card.h"
#include <array>

Card::Card(int value, Suit suit)
    : m_value(value), m_suit(suit) {}

/* ---------- simple getters ---------- */

int Card::getValue() const
{
    return m_value;
}

/**
 * Suit name in plain English — order must mirror the Suit enum!
 *   0 -> hearts, 1 -> diamonds, 2 -> clubs, 3 -> spades
 */
QString Card::getSuit() const
{
    static const std::array<QString,4> words{ "hearts",
                                              "diamonds",
                                              "clubs",
                                              "spades" };
    return words[static_cast<int>(m_suit)];
}

/**
 * Returns a file-name-ready ID such as
 *   "2_of_clubs", "jack_of_hearts", "ace_of_spades"
 * so that QPixmap(":/cards/" + getName() + ".png") finds the PNG.
 */
QString Card::getName() const
{
    static const std::array<QString,4> suitWords{ "hearts",
                                                  "diamonds",
                                                  "clubs",
                                                  "spades" };

    QString valueStr;
    switch (m_value) {
    case 1:  valueStr = "ace";   break;
    case 11: valueStr = "jack";  break;
    case 12: valueStr = "queen"; break;
    case 13: valueStr = "king";  break;
    default: valueStr = QString::number(m_value);
    }

    return valueStr + "_of_" + suitWords[static_cast<int>(m_suit)];
}

/* Unique 0-51 index (handy for sorting or serialising) */
int Card::getNumber() const
{
    return static_cast<int>(m_suit) * 13 + (m_value - 1);
}
