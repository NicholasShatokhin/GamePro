#include "gamefield.h"

GameField::GameField(QWidget *parent) :
    ClickableWidget(parent)
{
}

void GameField::paintEvent(QPaintEvent *)   // событие отрисовки игрового поля
{
    QPainter painter;   // рисовальщик
    painter.begin(this);    // рисуем на поверхности виджета

    if(!gameInProgress) // если игра еще не началась
    {
        QPixmap pixmap(":/rules.jpg");  // то загружаем картинку с правилами
        painter.drawPixmap(0, 0, width(), height(), pixmap);    // и рисуем эту картинку
    }
    else    // иначе
    {
        QImage image;
        image.load(":/one.jpg");    // загружаем изображение фишки
        image = image.scaled(wone, hone);   // изменяем ее размер под требуемый

        for (int i=0; i<k; i++) // для каждого столбца
        {
            painter.setPen(Qt::red);    // устанавливаем красный цвет пера
            painter.drawText(lft+(int)(wone/2.5)+(space+wone)*i, top+hone+15, QString::number((*one)[i]));  // выводим количество фишек в столбце

            for (int j=0; j<chmax; j++) // для каждой фишки
            {
                if (j<(*one)[i])    // если номер фишки меньше количства фишек в столбце
                {
                    painter.drawImage(lft+(space+wone)*i, top-(10+hone)*j, image);  // то рисуем эту фишку
                }
                else    // иначе все фишки столбца отрисованы
                    break;  // выходим из цикла
            }
        }
    }

    painter.end();  // завершаем процесс рисования
}
