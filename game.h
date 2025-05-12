#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "deck.h"

// Controls game flow and scoring
class Game {
public:
    Game();

    void startGame(); // Resets deck and shuffles
    bool dealNextRound(); // Returns false if no more rounds (cards in deck < 10 left)

    void playerSwap(const std::vector<int>& idx);
    bool m_swapDone = false; // Has player swapped this round?
    void rescoreAfterSwap(); // Recompute winner & scores

    const Player& winnerOfRound() const;
    const Player& overallWinner() const;
    int currentRound() const;

    void resetGame();


    const Player& player() const {
        return m_player;
    }
    const Player& computer() const {
        return m_computer;
    }
    Player& player() {
        return m_player;
    }
    Player& computer() {
        return m_computer;
    }
    const Deck& deck() const {
        return m_deck;
    }

private:
    Deck m_deck;
    Player m_player;
    Player m_computer;
    int m_round {0}; // 0 before first deal, then 1-5
    const Player* m_roundWinner {nullptr};
};


#endif
