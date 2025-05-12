#include "game.h"
#include <QMap>

Game::Game() : m_player("You"), m_computer("Computer") {
}

void Game::startGame() {
    m_deck.reset();
    m_deck.shuffle();

    m_player.resetScore();
    m_computer.resetScore();

    m_round = 0;
    m_roundWinner = nullptr;
}

// Deals two hands of 5 cards each, returns false if deck does not have five cards remaining
bool Game::dealNextRound()
{
    if (m_deck.cardsRemaining() < 10)
        return false;

    // Deal
    m_player.hand().dealHand(m_deck);
    m_computer.hand().dealHand(m_deck);

    // Check dealt cards
    auto strength = [](const QString& code) -> int {
        // inline table: higher number = stronger
        static const QMap<QString,int> t = {{"high",1}, {"pair",2}, {"twop",3}, {"trio",4}, {"strt",5}, {"flsh",6}, {"full",7}, {"four",8}, {"stfl",9}, {"ryfl",10}};
        return t.value(code,0);
    };

    const QString pBest = m_player.hand().getBest();
    const QString cBest = m_computer.hand().getBest();

    int pRank = strength(pBest);
    int cRank = strength(cBest);

    // Decide winner and update score
    auto highCardBreak = [&](bool& pWins, bool& cWins) {
        int pVals[5], cVals[5];
        for (int i = 0; i < 5; ++i) {
            pVals[i] = m_player.hand().cards()[i].getValue();
            cVals[i] = m_computer.hand().cards()[i].getValue();
        }
        std::sort(pVals, pVals + 5);
        std::sort(cVals, cVals + 5);

        for (int i = 4; i >= 0; --i) {
            if (pVals[i] > cVals[i]) { pWins = true; break; }
            if (cVals[i] > pVals[i]) { cWins = true; break; }
        }
    };


    bool pWins = (pRank > cRank);
    bool cWins = (cRank > pRank);

    if (!pWins && !cWins)
        highCardBreak(pWins,cWins);

    if (pWins) {
        m_player.incrementScore();
        m_roundWinner = &m_player;
    } else if (cWins) {
        m_computer.incrementScore();
        m_roundWinner = &m_computer;
    } else { // exact draw – favour human -  incorrect need to change
        m_roundWinner = &m_player;
    }

    ++m_round;
    return true;
}


void Game::playerSwap(const std::vector<int>& idx)
{
    m_player.hand().swapCards(idx, m_deck);
}



const Player& Game::winnerOfRound() const
{
    return *m_roundWinner;  // Pointer set inside scoreRound()
}



const Player& Game::overallWinner() const
{
    if (m_player.getScore() > m_computer.getScore())
        return m_player;
    if (m_computer.getScore() > m_player.getScore())
        return m_computer;
    // If it’s a tie, by convention return the human player
    return m_player;
}

// Almost identical to dealNextRound however only replaces cards and recalculates values
void Game::rescoreAfterSwap()
{
    // ── 1.  Recompute which hand is stronger ─────────────────────────
    auto strength = [](const QString& code) -> int {
        static const QMap<QString,int> t = {{"high",1}, {"pair",2}, {"twop",3}, {"trio",4}, {"strt",5}, {"flsh",6}, {"full",7}, {"four",8}, {"stfl",9}, {"ryfl",10}};
        return t.value(code,0);
    };

    const QString pBest = m_player.hand().getBest();
    const QString cBest = m_computer.hand().getBest();
    int pRank = strength(pBest);
    int cRank = strength(cBest);

    auto highCardBreak = [&](bool& pWins, bool& cWins) {
        int pVals[5], cVals[5];
        for (int i = 0; i < 5; ++i) {
            pVals[i] = m_player.hand().cards()[i].getValue();
            cVals[i] = m_computer.hand().cards()[i].getValue();
        }
        std::sort(pVals, pVals + 5);
        std::sort(cVals, cVals + 5);

        for (int i = 4; i >= 0; --i) {
            if (pVals[i] > cVals[i]) {
                pWins = true;
                break;
            }

            if (cVals[i] > pVals[i]) {
                cWins = true;
                break;
            }
        }
    };


    bool pWins = (pRank > cRank);
    bool cWins = (cRank > pRank);
    if (!pWins && !cWins) {
        highCardBreak(pWins,cWins);
    }

    Player* newWinner = nullptr;
    if (pWins) {
        newWinner = &m_player;
    } else if (cWins) {
        newWinner = &m_computer;
    } else {
        newWinner = &m_player;
    }

    // This handles if the winner changes, either player becomes winner or loser based on their swaps
    if (newWinner != m_roundWinner) {
        if (m_roundWinner == &m_player) {
            m_player.decrementScore();
        }
        if (m_roundWinner == &m_computer) {
            m_computer.decrementScore();
        }
        if (newWinner == &m_player) {
            m_player.incrementScore();
        }
        if (newWinner == &m_computer) {
            m_computer.incrementScore();
        }
        m_roundWinner = newWinner;
    }
}


int Game::currentRound() const
{
    return m_round;
}


void Game::resetGame()
{
    startGame();
}


