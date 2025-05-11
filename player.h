#ifndef PLAYER_H
#define PLAYER_H

#include <QString>
#include "hand.h"


// Handles player logic
class Player
{
public:
    // Player name
    explicit Player(const QString& name = "Player");

    // Scoring functions
    void resetScore();        // set score to 0
    void incrementScore();    // +1 to score
    int  getScore() const;    // current score

    // Gets player type
    QString getName() const;  // Computer / User etc

    // Hand accessers
    Hand& hand();       // non-const ref for game logic to mutate
    const Hand& hand() const; // const ref for read-only UI display

private:
    QString m_name;
    int m_score {0};
    Hand m_hand;
};

#endif // PLAYER_H
