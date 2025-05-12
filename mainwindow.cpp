#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <algorithm>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* list of the ten card labels in grid order */
    m_slots = { ui->card11, ui->card12, ui->card13, ui->card14, ui->card15,
               ui->card21, ui->card22, ui->card23, ui->card24, ui->card25 };

    /* button signals */
    connect(ui->StartBtn, &QPushButton::clicked,
            this,          &MainWindow::onBtnStart);
    connect(ui->NextBtn,  &QPushButton::clicked,
            this,          &MainWindow::onBtnNext);

    // Swap
    connect(ui->SwapBtn, &QPushButton::clicked,
            this,         &MainWindow::onBtnSwap);

    ui->SwapBtn->setEnabled(false);


    /* optional: card click signal (does nothing yet) */
    for (CardLabel *lbl : m_slots)
        connect(lbl, &CardLabel::clicked,
                this, &MainWindow::onCardClicked);

    ui->NextBtn->setEnabled(false);   // disabled until first deal
    refresh();                        // show backs / zero score
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* ---------- Start / Deal ---------- */
void MainWindow::onBtnStart()
{
    m_game.startGame();
    m_game.dealNextRound();

    m_swapDoneThisRound = false;
    ui->NextBtn->setEnabled(true);
    refresh();
}

/* ---------- Next Round ------------ */
void MainWindow::onBtnNext()
{
    if (!m_game.dealNextRound())          // deck < 10 → stop
        ui->NextBtn->setEnabled(false);

    m_swapDoneThisRound = false;
    refresh();
}

// Swap
void MainWindow::onCardClicked(CardLabel *lbl)
{
    if (m_swapDoneThisRound) return;

    // A) Figure out which label
    int guiIndex = std::find(m_slots.begin(), m_slots.end(), lbl)
                   - m_slots.begin();

    // Ignore clicks on the computer row
    if (guiIndex < 5) return;

    int handIdx = guiIndex - 5;                 // 0-4

    // B) Only allow in rounds 1-4 and max 3 cards
    if (m_game.currentRound() > 4) return;
    bool already = m_selected[handIdx];

    if (!already && m_queue.size() == 3) return;   // cap reached

    // C) Toggle highlight
    m_selected[handIdx] = !already;
    if (m_selected[handIdx]) {
        m_queue.push_back(handIdx);
        lbl->setStyleSheet("border: 2px solid gold;");
    } else {
        m_queue.erase(std::remove(m_queue.begin(), m_queue.end(), handIdx),
                      m_queue.end());
        lbl->setStyleSheet("");                  // clear border
    }

    // D) Enable/disable Swap button
    ui->SwapBtn->setEnabled(!m_queue.empty());
}


void MainWindow::onBtnSwap()
{
    m_game.playerSwap(m_queue);
    m_game.rescoreAfterSwap();
    m_swapDoneThisRound = true;

    // wipe local state & UI
    m_queue.clear();
    m_selected.fill(false);
    ui->SwapBtn->setEnabled(false);
    for (int i = 5; i < 10; ++i)          // clear all borders
        m_slots[i]->setStyleSheet("");

    refresh();                            // redraw both hands & labels
}



/* ---------- Paint everything ------ */
void MainWindow::refresh()
{
    m_selected.fill(false);
    m_queue.clear();
    ui->SwapBtn->setEnabled(false);
    for (int i = 5; i < 10; ++i)
        m_slots[i]->setStyleSheet("");



    const QString prefix = ":/cards";     // resource prefix

    /* computer row */
    const auto &cpu = m_game.computer().hand().cards();
    for (int i = 0; i < 5; ++i)
        if (cpu.empty())
            m_slots[i]->setPixmap(QPixmap(":/cards/back.png"));
        else
            m_slots[i]->setCard(cpu[i], prefix);

    /* player row */
    const auto &human = m_game.player().hand().cards();
    for (int i = 0; i < 5; ++i)
        if (human.empty())
            m_slots[5 + i]->setPixmap(QPixmap(":/cards/back.png"));
        else
            m_slots[5 + i]->setCard(human[i], prefix);

    /* scoreboard */
    ui->ScoreLbl->setText(
        QString("You: %1  -  Computer: %2")
            .arg(m_game.player().getScore())
            .arg(m_game.computer().getScore()));

    /* status line */
    if (m_game.currentRound() == 0)
        ui->WelcomeLbl->setText("Click \"Start\" to begin");
    else
        ui->WelcomeLbl->setText(
            QString("%1 wins round with %2")
                .arg(m_game.winnerOfRound().getName())
                .arg(m_game.winnerOfRound().hand().getBest()));
}
