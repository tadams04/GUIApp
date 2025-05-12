#include "card.h"
#include <array>

Card::Card(int value, Suit suit) : m_value(value), m_suit(suit) {}

int Card::getValue() const {
    return m_value;
}


// Suit name in english
QString Card::getSuit() const {
    static const std::array<QString,4> words{ "hearts", "diamonds", "clubs", "spades" };
    return words[static_cast<int>(m_suit)];
}

// Returns file name ready for getting png card images
QString Card::getName() const {
    static const std::array<QString,4> suitWords{ "hearts", "diamonds", "clubs", "spades" };

    QString valueStr;
    if (m_value == 1) {
        valueStr = "ace";
    }
    else if (m_value == 11) {
        valueStr = "jack";
    }
    else if (m_value == 12) {
        valueStr = "queen";
    }
    else if (m_value == 13) {
        valueStr = "king";
    }
    else {
        valueStr = QString::number(m_value);
    }

    // We ignore the alternative face cards
    return valueStr + "_of_" + suitWords[static_cast<int>(m_suit)];
}

// 0-51 index
int Card::getNumber() const {
    return static_cast<int>(m_suit) * 13 + (m_value - 1);
}
