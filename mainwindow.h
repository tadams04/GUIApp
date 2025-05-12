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
    void onBtnStart();            // “Start / Deal”
    void onBtnNext();             // “Next Round”
    void onCardClicked(CardLabel*);

private:
    void refresh();               // draw cards, score, status

    Ui::MainWindow      *ui;
    Game                 m_game;
    QVector<CardLabel*>  m_slots; // 10 card widgets, computer first row
};

#endif // MAINWINDOW_H
