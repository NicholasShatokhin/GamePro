#ifndef REGISTRATION_H
#define REGISTRATION_H

#include <QMessageBox>
#include <QPainter>
#include <QWidget>

namespace Ui {
class Registration;
}

class Registration : public QWidget
{
    Q_OBJECT
    
public:
    explicit Registration(QWidget *parent = 0);
    ~Registration();

    void paintEvent(QPaintEvent *); // событие перерисовки виджета
    
public slots:
    void clearForm();   // очистка полей формы

private:
    Ui::Registration *ui;

private slots:
    void onRegistration();  // нажата кнопка регистрации. Проверяет корректность введенных данных

signals:
    void doRegistration(QString name, QString password);    // посылается при нажатии на кнопку регистрации при корретных введенных данных
};

#endif // REGISTRATION_H
