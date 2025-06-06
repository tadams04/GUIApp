#ifndef CARDLABEL_H
#define CARDLABEL_H

#include <QLabel>
#include "card.h"

// QLabel to store card and click
class CardLabel : public QLabel {
    Q_OBJECT
public:
    explicit CardLabel(QWidget *parent = nullptr);

    void setCard(const Card &c, const QString &prefix = ":/cards"); // updates pixmap
    const Card &card() const { return m_card; }

signals:
    void clicked(CardLabel *); // emitted on mouse click

protected:
    void mousePressEvent(QMouseEvent *ev) override;

private:
    Card m_card;
};

#endif
