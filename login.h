#ifndef LOGIN_H
#define LOGIN_H

#include <QMessageBox>
#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT
    
public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    
private:
    Ui::Login *ui;

private slots:
    void onLogin(); // нажатие на кнопку входа. Проверка корректности введенных данных

signals:
    void doLogin(QString name, QString password);   // посылается при нажатии на кнопку входа при корректных введенных данных
    void doRegistration();  // нажатие на кнопку регистрации
    void doExit();  // нажатие на кнопку выхода

};

#endif // LOGIN_H
