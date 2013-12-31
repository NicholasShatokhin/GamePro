#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

#include "gamecontroller.h"

// абстрактный класс
class ClickableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ClickableWidget(QWidget *parent = 0);
    virtual ~ClickableWidget();

    // события нажатия и отпускания кнопок мыши
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

    // событие перерисовки виджета. Чисто виртуальная функция
    virtual void paintEvent(QPaintEvent *) = 0;

    // дергается при запуске игры. Нужно чтобы заменить картинку с правилами на игровое поле
    void gameStarted();
    
signals:
    void clicked(int x, int y); // посылается при клике на виджет. В параметрах координаты клика
    
public slots:   // установка параметров игрового поля
    void setSizes(int _lft, int _top, int _wone, int _hone, int _space, int _k, int _chmax, game *_one);

protected:  // параметры игрового поля
    int lft;
    int top;
    int wone;
    int hone;
    int space;
    int k;
    int chmax;
    game * one;

    bool gameInProgress;
};

#endif // CLICKABLELABEL_H
