#include "clickablewidget.h"

ClickableWidget::ClickableWidget(QWidget *parent) :
    QWidget(parent)
{   // обнуляем все параметры
    lft = 0;
    top = 0;
    wone = 0;
    hone = 0;
    space = 0;
    k = 0;
    chmax = 0;

    gameInProgress = false;
}

ClickableWidget::~ClickableWidget()
{
}

void ClickableWidget::mousePressEvent(QMouseEvent *)
{
}

void ClickableWidget::mouseReleaseEvent(QMouseEvent *ev) // если мышка была отпущена над этим виджетом, значит по нему был произведен клик
{
    emit clicked(ev->x(), ev->y()); // посылаем координаты клика
}

void ClickableWidget::gameStarted() // игра началась
{
    gameInProgress = true;
}


void ClickableWidget::setSizes(int _lft, int _top, int _wone, int _hone, int _space, int _k, int _chmax, game * _one)
{   // устанавливаем параметры
    lft = _lft;
    top = _top;
    wone = _wone;
    hone = _hone;
    space = _space;
    k = _k;
    chmax = _chmax;
    one = _one;
}
