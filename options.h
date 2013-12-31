#ifndef OPTIONS_H
#define OPTIONS_H

#include <QPainter>
#include <QWidget>

namespace Ui {
class Options;
}

class Options : public QWidget
{
    Q_OBJECT
    
public:
    explicit Options(QWidget *parent = 0);
    ~Options();
    
    void paintEvent(QPaintEvent *); // событие перерисовки виджета

    void emitParams();  // при вызове посылает сигнал с текущими настройками

private:
    Ui::Options *ui;

private slots:
    void onClose(); // нажата кнопка закрытия окна

signals:
    void saveParams(int k_min, int k_max, int m_min, int m_max, int player);    // сигнал с текущими настройками

};

#endif // OPTIONS_H
