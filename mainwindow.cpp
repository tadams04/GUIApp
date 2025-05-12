#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* ------------------------------------------------------
     *  Gather the 10 CardLabel* in the *left-to-right* order
     *  we want to display cards.
     *  Your naming is:
     *     Row-0 : label5 label4 label3 label2 label1
     *     Row-1 : label10 label9 label8 label7 label6
     * -----------------------------------------------------*/
    m_slots = { ui->card11, ui->card12, ui->card13, ui->card14, ui->card15,
               ui->card21, ui->card22, ui->card23, ui->card24, ui->card25 };

    /* connect button signals */
    connect(ui->StartBtn, &QPushButton::clicked,
            this, &MainWindow::onBtnStart);
    connect(ui->NextBtn,  &QPushButton::clicked,
            this, &MainWindow::onBtnNext);

    /* connect click on every card label */
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

/* ---------- Start / Deal button ---------- */
void MainWindow::onBtnStart()
{
    m_game.startGame();
    m_game.dealNextRound();

    ui->NextBtn->setEnabled(true);
    refresh();
}

/* ---------- Next Round button ------------ */
void MainWindow::onBtnNext()
{
    if (!m_game.dealNextRound()) {           // deck < 10 → game over
        ui->NextBtn->setEnabled(false);
        ui->WelcomeLbl->setText("Game finished – click Start for new game");
    }
    refresh();
}

/* ---------- (future) card-click handler --- */
void MainWindow::onCardClicked(CardLabel* lbl)
{
    /* Swap logic will go here later.
       For now we just flash the border so you see clicks registering. */
    lbl->setStyleSheet("border: 2px solid red");
}

/* ---------- Draw everything --------------- */
void MainWindow::refresh()
{
    const QString prefix = ":/cards";        // resource prefix

    /* computer row (m_slots 0-4) */
    const auto& cpu = m_game.computer().hand().cards();
    for (int i = 0; i < 5; ++i) {
        if (cpu.empty())
            m_slots[i]->setPixmap(QPixmap(":/cards/back.png"));
        else
            m_slots[i]->setCard(cpu[i], prefix);
    }

    /* player row (m_slots 5-9) */
    const auto& human = m_game.player().hand().cards();
    for (int i = 0; i < 5; ++i) {
        if (human.empty())
            m_slots[5+i]->setPixmap(QPixmap(":/cards/back.png"));
        else
            m_slots[5+i]->setCard(human[i], prefix);
    }

    /* scoreboard */
    ui->ScoreLbl->setText(
        QString("You: %1  -  Computer: %2")
            .arg(m_game.player().getScore())
            .arg(m_game.computer().getScore()));

    /* status / welcome */
    if (m_game.currentRound() == 0)
        ui->WelcomeLbl->setText("Click \"Start\" to begin");
    else
        ui->WelcomeLbl->setText(
            QString("%1 wins round with %2")
                .arg(m_game.winnerOfRound().getName())
                .arg(m_game.winnerOfRound().hand().getBest()));
}
