#include "card.h"

static const std::array<QString,4> SUIT_SYMBOLS{QStringLiteral(u"♥"), QStringLiteral(u"♦"), QStringLiteral(u"♣"), QStringLiteral(u"♠")};

Card::Card(int value, Suit suit)
    : m_value(value), m_suit(suit) {}

int Card::getValue() const
{
    return m_value;
}

QString Card::getSuit() const
{
    return SUIT_SYMBOLS.at(static_cast<int>(m_suit));
}

QString Card::getName() const
{
    // Can rewrite using swithc later
    if (m_value == 1) {
        return QStringLiteral(u"A%1").arg(getSuit());
    } else if (m_value == 11) {
        return QStringLiteral(u"J%1").arg(getSuit());
    } else if (m_value == 12) {
        return QStringLiteral(u"Q%1").arg(getSuit());
    } else if (m_value == 13) {
        return QStringLiteral(u"K%1").arg(getSuit());
    } else {
        return QStringLiteral(u"%1%2").arg(m_value).arg(getSuit());
    }
}

int Card::getNumber() const
{
    // Maps Hearts 1..13 → 0‑12, Diamonds → 13‑25, etc.
    return static_cast<int>(m_suit) * 13 + (m_value - 1);
}
