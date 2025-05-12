#include "player.h"


Player::Player(const QString& name)
    : m_name(name)            // member-initializer list
{}

// Scoring
void Player::resetScore() {
    m_score = 0;
}

void Player::incrementScore() {
    ++m_score;
}

// Swap helper
void Player::decrementScore()
{
    --m_score;
}


int  Player::getScore() const {
    return m_score;
}


// Gets player name
QString Player::getName() const {
    return m_name;
}


// Accessing hands
Hand& Player::hand() {
    return m_hand;
}

const Hand& Player::hand() const {
    return m_hand;
}
