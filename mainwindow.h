#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

#include "cardlabel.h"
#include "game.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onBtnStart();            // Start / Deal
    void onBtnNext();             // Next Round
    void onCardClicked(CardLabel*);
    void onBtnSwap();


private:
    void refresh();               // draw cards, score, status

    Ui::MainWindow   *ui;
    Game              m_game;
    QVector<CardLabel*> m_slots;  // 10 card widgets in grid order
    QVector<CardLabel*> m_selected; // for future swap feature
    QVector<int> m_selectedSlots;     // indexes 0-4 computer, 5-9 human

};

#endif // MAINWINDOW_H
