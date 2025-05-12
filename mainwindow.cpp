#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

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

    ui->NextBtn->setEnabled(true);
    refresh();
}

/* ---------- Next Round ------------ */
void MainWindow::onBtnNext()
{
    if (!m_game.dealNextRound())          // deck < 10 → stop
        ui->NextBtn->setEnabled(false);

    refresh();
}

/* ---------- Card clicked ---------- */
void MainWindow::onCardClicked(CardLabel *)
{
    /* original version: no swapping, nothing to do */
}

/* ---------- Paint everything ------ */
void MainWindow::refresh()
{
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
