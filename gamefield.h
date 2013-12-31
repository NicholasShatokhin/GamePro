#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "clickablewidget.h"

class GameField : public ClickableWidget
{
    Q_OBJECT
public:
    explicit GameField(QWidget *parent = 0);

    // событие перерисовки виджета
    void paintEvent(QPaintEvent *);
    
signals:
    
public slots:
    
};

#endif // GAMEFIELD_H
