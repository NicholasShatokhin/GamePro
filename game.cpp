#include "game.h"
#include "ui_game.h"

Game::Game(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    // скроем поле выбора количества фишек
    ui->turnWeight->setVisible(false);
    // и кнопку завершения хода
    ui->endTurnBtn->setVisible(false);
    ui->endTurnBtn->setEnabled(false);

    // параметры столбцов на форме
    wone=60;
    hone=20;
    lft=30;
    top=300;
    space=16;
    chmax=10; // максимум блоков в столбце
    cwmax=7;  // максимум количество столбцов
    mns=1;    // уменьшение столбца по клику человека

    ps=3;
    process=-1;

    // назначим слот сигналу нажатия на кнопку "Завершение хода"
    connect(ui->endTurnBtn, SIGNAL(clicked()), this, SLOT(endTurn()));
    // назначим слот сигналу нажатия на игровое поле
    connect(ui->field, SIGNAL(clicked(int,int)), this, SLOT(imageClicked(int,int)));
}

Game::~Game()
{
    delete ui;
}

void Game::setName(QString name)
{
    _name = name;   // установим имя игрока
}

void Game::startGame()  // начало игры
{
    ui->turnWeight->setVisible(true);   // покажем поле выбора количества снимаемых фишек
    ui->endTurnBtn->setVisible(true);   // покажем кнопку завершения хода
    ui->endTurnBtn->setEnabled(true);   // и сделаем ее активной

    process=-1;

    one.set_gen(kmin,kmax,mmin,mmax,pl);    // устанавливаем параметры игры
    one.gen();  // генерируем начальное расположение фишек

    int k = one.getk(); // количество столбцов

    wone =  (577-lft)/k-space;  // расчитываем ширину фишки

    ui->field->setSizes(lft, top, wone, hone, space, k, chmax, &one);   // устанавливаем параметры игрового поля
    ui->field->gameStarted();   // и сообщаем ему, что игра началась

    drawField();    // рисуем игровое поле
}

void Game::setParams(int k_min, int k_max, int m_min, int m_max, int player)
{   // устанавливаем параметры из настроек
    kmin = k_min;
    kmax = k_max;
    mmin = m_min;
    mmax = m_max;
    pl = player;
}

int Game::min(int a, int b) // функция минимума
{
    if(a < b)
        return a;
    else
        return b;
}

void Game::drawField()
{
    ui->field->repaint();   // принудительно вызываем событие перерисовки виджета с игровым полем
}

void Game::endTurn()    // нажата кнопка завершения хода
{
    if (process==-1)
     return;
    if (one.is_end())   // если ход был последним
    {
        QMessageBox(QMessageBox::Information, tr("Победа!"), tr("Вы выиграли!")).exec();    //выводим поздравления
        emit updateStat(_name, 1);  // записываем победу в статистику
        one.setplayer(3);   // никто не ходит следующим
        return;
    }
    else    // иначе
    {
        process=-1;
        one.setplayer(0);   // ходит компьютер
        int count;
        int p=one.next(count);  // делаем ход компьютером
        if (p==-1)  // если вернулась -1, то ходил не компьютер или он ходил после завершения игры. Это невозможно
        {
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Странная ошибка!")).exec();
            return; // выходим
        }

        if (one.is_end())   // если это был последний ход
        {
            one.setplayer(3);   // то никто не ходит следующим

            emit updateStat(_name, -1); // записываем игроку поражение
            QMessageBox(QMessageBox::Information, tr("Поражение"), tr("Вы проиграли")).exec(); // и выводим сообщение об этом
            return;
        }
        else    // иначе
        {
           one.setplayer(1);    // следующим ходит игрок

           drawField(); // отрисовываем поле
        }

    }
}

void Game::imageClicked(int x, int y)   // при клике на игровое поле
{
    int itemCol = -1;
    int itemNum = -1;

    for (int i=0; i<one.getk(); i++)    // пройдемся
    {                                   // по всем
        for (int j=0; j<chmax; j++)     // фишкам
        {   // если кликнули на эту фишку
            if(j < one[i] && x >= lft+(space+wone)*i && y >= top-(10+hone)*j
                    && x<= lft+(space+wone)*i + wone && y <= top-(10+hone)*j + hone)
            {
                itemCol = i;    // сохраняем ее координаты
                itemNum = j;
                break;  // и выходим из цикла
            }
        }
    }
    // если клик был произведен не по фишке
    if(itemCol < 0 || itemNum < 0)  return; // то выходим из функции

    if (one.getplayer()==0 && !one.is_end())    // если ходит компьютер и игра не окончена
    {   // выводим сообщение об этом
        QMessageBox(QMessageBox::Information, tr("Игра началась"), tr("Игра началась. Компьютер делает первый ход")).exec();
                process=-1;
                    one.setplayer(0);   // ходит компьютер
                    int count;
                    int p=one.next(count);  // компьютер делает ход
                    if (p==-1)  // невозможно это
                    {
                        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Странная ошибка!")).exec();
                        return;
                    }

                    if (one.is_end())   // если это был последний ход
                    {
                            one.setplayer(3);   // то следующим никто не ходит
                            emit updateStat(_name, -1); // запишем игроку поражение
                            QMessageBox(QMessageBox::Information, tr("Поражение"), tr("Вы проиграли")).exec(); // выведем сообщение об этом
                            return; // и выйдем из функции
                    }

                    drawField();    // перерисуем игровое поле
    }
    else if (one.getplayer()==1 && !one.is_end())   // если ходит игрок
    {
        mns = ui->turnWeight->value();  // то возьмем количество снимаемых фишек
        if (one.getplayer()==1 && one[itemCol]>0 && (process==-1 || process==itemCol))
        {
             process=itemCol;

             if (one[itemCol]-mns<0)    // если игрок пытается снять больше фишем, чем есть в столбце
             {
               one[itemCol]=0;  // то снимем все оставшиеся
             }
             else   // иначе снимаем, сколько он хочет
               one[itemCol]=one[itemCol]-mns;


             if (one.is_end())  // если он снял все фишки
             {  // то поздравим его с победой
                 QMessageBox(QMessageBox::Information, tr("Победа"), tr("Вы выиграли!")).exec();
                 emit updateStat(_name, 1); // обновим статистику
                 one.setplayer(3);  // никто дальше не ходит
                 return;    // выходим
             }

             if (one[itemCol]==0 && !one.is_end() && one.getplayer()==1)    // если ходил игрок, фишки в столбце закончились, но на поле нет
             {
                    one.setplayer(0);   // то ходит компьютер

                    process=-1;
                    int count;
                    int p=one.next(count);  // ход копьютера

                    if (p==-1)
                    {
                        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Странная ошибка!")).exec();
                        return;
                    }

                    if (one.is_end())   // если фишки закончились
                    {
                            one.setplayer(3);   // то дальше никто не ходит

                            emit updateStat(_name, -1); // запишем поражение в статистику
                            QMessageBox(QMessageBox::NoIcon, tr("Поражение"), tr("Вы проиграли")).exec();   // выведем сообщение
                            process=-1;
                            return; // и выйдем
                    }
             }

             drawField();   // перерисуем поле
        }
    }
}
