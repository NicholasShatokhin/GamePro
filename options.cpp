#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    // назначим слот сигналу нажатия на кнопку закрытия
    connect(ui->closeBtn,SIGNAL(clicked()), this, SLOT(onClose()));
}

Options::~Options()
{
    delete ui;
}

void Options::paintEvent(QPaintEvent * ev)
{
    QPixmap pixmap(":/background.jpg"); // загружаем фоновую картинку
    QPainter painter(this); // рисуем на поверхности окна

    painter.drawPixmap(0, 0, width(), height(), pixmap);    // рисуем картинку

    QWidget::paintEvent(ev);    // рисуем остальную часть окна
}

void Options::emitParams()
{
    int player;

    if(ui->computerRBtn->isChecked())  // если включена радиокнопка "Компьютер"
            player = 0; // то первым ходит человек
    else if(ui->humanRBtn->isChecked())  // если "Человек"
        player = 1; // то компьютер
    else
        player = rand() % 2;    // иначе выберем случайно
    // посылаем сигнал с выбранными настройками
    emit saveParams(ui->groupsFromBox->value(), ui->groupsToBox->value(), ui->itemsFromBox->value(), ui->itemsToBox->value(), player);
}

void Options::onClose() // нажата кнопка закрытия
{
    emitParams();   // посылаем сигнал с настройками

    close();    // и закрываем окно
}
