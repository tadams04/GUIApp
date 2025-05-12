#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "hand.h"


// Handles player, score and hand
class Player
{
public:
    // Player name
    explicit Player(const QString& name = "Player");

    // Scoring functions
    void resetScore();
    void incrementScore();
    void decrementScore();

    QString getName() const;  // Computer / User etc
    int getScore() const;

    // Hand accessers
    Hand& hand();
    const Hand& hand() const;

private:
    QString m_name;
    int m_score {0};
    Hand m_hand;
};

#endif
