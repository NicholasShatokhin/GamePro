#ifndef GAME_H
#define GAME_H

#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <QWidget>

#include "gamecontroller.h"

namespace Ui {
class Game;
}

class Game : public QWidget
{
    Q_OBJECT
    
public:
    explicit Game(QWidget *parent = 0);
    ~Game();

    // установка имени игрока. Необходимо для обновления статистики
    void setName(QString name);

public slots:
    void startGame();   // запуск игрового процесса
    void setParams(int k_min, int k_max, int m_min, int m_max, int player); // установка параметров игры из настроек
    
private:
    Ui::Game *ui;

    QString _name; // имя игрока

    game one;

    // параметры столбцов на форме
    int wone;   // ширина игровой фишки
    int hone;   // высота игровой фишки
    int lft;    // отступ слева
    int top;    // отступ сверху
    int space;  // расстояние между плашками
    int chmax; // максимум блоков в столбце
    int cwmax;  // максимум количество столбцов
    int mns;    // уменьшение столбца по клику человека

    int ps;
    int process;

    int kmin,kmax,mmin,mmax,pl; // параметры, задаваемые в настройках

    int min(int a, int b);  // функция минимума

    void drawField();   // функция, вызывающая перерисовку игрового поля

private slots:
    void endTurn(); // нажатие на кнопку "Завершить ход"
    void imageClicked(int x, int y);    // клик по игровому полю

signals:
    void updateStat(QString name, int k);   // посылается для обновления статистики

};

#endif // GAME_H
