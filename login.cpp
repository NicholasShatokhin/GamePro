#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    // назначим слоты сигналам нажатия на кнопки
    connect(ui->enterBtn, SIGNAL(clicked()), this, SLOT(onLogin()));
    connect(ui->regBtn, SIGNAL(clicked()), this, SIGNAL(doRegistration()));
    connect(ui->exitBtn, SIGNAL(clicked()), this, SIGNAL(doExit()));
}

Login::~Login()
{
    delete ui;
}

void Login::onLogin()
{   // если не введены логин или пароль
    if(ui->nameFld->text().trimmed().isEmpty() || ui->passwordFld->text().trimmed().isEmpty())
    {   // то покажем сообщение об ошибке
        QMessageBox message(QMessageBox::Critical, tr("Ошибка"), tr("Данные для входа указаны неверно"));
        message.exec();
    }
    else
    {   // иначе посылаем сигнал с введенными данными
        emit doLogin(ui->nameFld->text().trimmed(), ui->passwordFld->text().trimmed());
    }
}
