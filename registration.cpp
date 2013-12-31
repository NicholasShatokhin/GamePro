#include "registration.h"
#include "ui_registration.h"

Registration::Registration(QWidget *parent) :
    QWidget(parent, Qt::Window),
    ui(new Ui::Registration)
{
    ui->setupUi(this);

    setWindowModality(Qt::ApplicationModal);    // делаем окно модальным - пока его не закроют, работать с другими окнами приложение не даст
    // назначим слоты для сигналов нажатия на кнопки регистрации и отмены
    connect(ui->acceptBtn, SIGNAL(clicked()), this, SLOT(onRegistration()));
    connect(ui->cancelBtn, SIGNAL(clicked()), this, SLOT(close()));
}

Registration::~Registration()
{
    delete ui;
}

void Registration::paintEvent(QPaintEvent * ev)
{
    QPixmap pixmap(":/background.jpg"); // загрузим фоновое изображение
    QPainter painter(this); // рисуем на поверхности окна

    painter.drawPixmap(0, 0, width(), height(), pixmap);    // рисуем изображение

    QWidget::paintEvent(ev);    // рисуем остальную часть окна
}

void Registration::clearForm()
{   // очищаем содержимое полей
    ui->nameFld->setText("");
    ui->passFld->setText("");
    ui->repeatPassFld->setText("");
}

void Registration::onRegistration()
{   // проверка корректности данных
    if(ui->nameFld->text().trimmed().isEmpty()) // если имя не введено
    {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не указано имя")).exec(); // покажем ошибку
    }
    else if(ui->passFld->text().trimmed().isEmpty())    // если пароль не введен
    {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не указан пароль")).exec();    // ошибка
    }
    else if(ui->passFld->text() != ui->repeatPassFld->text())   // ели пароли не совпадают
    {
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Пароли не совпадают")).exec(); // ошибка
    }
    else
    {
        emit doRegistration(ui->nameFld->text().trimmed(), ui->passFld->text().trimmed());  // если все верно, посылаем сигнал
    }
}
