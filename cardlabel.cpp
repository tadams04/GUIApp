#include "cardlabel.h"
#include <QMouseEvent>
#include <QPixmap>

CardLabel::CardLabel(QWidget *parent) : QLabel(parent)
{
    setScaledContents(true);
    // show back until real card set
    setPixmap(QPixmap(":/cards/back.png"));
}

void CardLabel::setCard(const Card &c, const QString &prefix)
{
    m_card = c;
    const QString path = prefix + "/" + c.getName() + ".png";
    setPixmap(QPixmap(path));               // will be valid after you deal
}

void CardLabel::mousePressEvent(QMouseEvent *ev)
{
    emit clicked(this);
    QLabel::mousePressEvent(ev);
}
