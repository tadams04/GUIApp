#include "game.h"
#include <QDebug> // Only for printing before GUI implementation
#include <QMap>

Game::Game() : m_player("You"), m_computer("Computer") {

}

void Game::startGame() {
    m_deck.reset();       // Rebuilds the 52-card deck
    m_deck.shuffle();     // Randomizes card order

    m_player.resetScore();    // Set human score back to 0
    m_computer.resetScore();  // Set computer score back to 0

    m_round = 0;              // No rounds played yet
    m_roundWinner = nullptr;  // No winner yet
}



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
        // force value-sort then compare top-down
        m_player.hand().sortValue();
        m_computer.hand().sortValue();
        const auto& pC = m_player.hand().cards();
        const auto& cC = m_computer.hand().cards();
        for (int i = 4; i >= 0; --i) {
            if (pC[i].getValue() > cC[i].getValue()) { pWins = true; break; }
            if (cC[i].getValue() > pC[i].getValue()) { cWins = true; break; }
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
    } else {                              // exact draw – favour human
        m_roundWinner = &m_player;
    }

    // Prints for debugging
    ++m_round;
    qDebug() << "Round" << m_round
             << ":" << pBest << '(' << m_player.getName()   << ") vs."
             << cBest << '(' << m_computer.getName() << ")  →"
             << "Winner:" << m_roundWinner->getName();

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



int Game::currentRound() const
{
    return m_round;
}



void Game::resetGame()
{
    startGame();
}


