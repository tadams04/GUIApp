#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <array>

#include "cardlabel.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBtnStart();
    void onBtnNext();
    void onCardClicked(CardLabel*);
    void onBtnSwap();


private:
    void refresh(); // Updates all widgets visuals

    Ui::MainWindow *ui;
    Game m_game;
    QVector<CardLabel*>  m_slots; // 10 card widgets, computer first row

    // Swap
    bool m_swapDoneThisRound = false;
    std::array<bool,5> m_selected {{false}}; // which player cards are lit
    std::vector<int>   m_queue; // actual indices to send to swap

};

#endif
