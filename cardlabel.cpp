#include "cardlabel.h"
#include <QMouseEvent>
#include <QPixmap>

CardLabel::CardLabel(QWidget *parent) : QLabel(parent) {
    setScaledContents(true); // Auto fits the image, also enabled in widget settings
    setPixmap(QPixmap(":/cards/back.png"));
}

void CardLabel::setCard(const Card &c, const QString &prefix) {
    m_card = c; // New card
    const QString path = prefix + "/" + c.getName() + ".png";
    setPixmap(QPixmap(path));
}

// Passes click to mainwindow
void CardLabel::mousePressEvent(QMouseEvent *ev) {
    emit clicked(this);
    QLabel::mousePressEvent(ev);
}
