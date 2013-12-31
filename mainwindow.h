#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QFile>
#include <QMainWindow>

#include "login.h"
#include "registration.h"
#include "game.h"
#include "statistics.h"
#include "statistics.cpp"
#include "options.h"

struct pwd   // структура для регистрации
{
  char pass[100];
  char login[100];
  int  id;
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent *);   // событие изменения размеров окна
    void closeEvent(QCloseEvent *); // событие закрытия окна
    
private:
    Ui::MainWindow *ui;

    Login * login;  // виджет с формой ввода логина и пароля для входа
    Registration * registration;    // виджет с формой регистрации
    Game * game;    // виджет с игровым полем
    Statistics<QWidget> * statistics;   // виджет статистики. Шаблонный класс
    Options * options;  // виджет с настройками

    bool entr;

    void switchToGame(QString name);    // скрыть виджет входа и показать виджет с игровым полем

private slots:
    void doLogin(QString name, QString password);   // пришли данные для входа
    void doRegistration();  // нажата кнопка регистрации
    void doRegistration(QString name, QString password);    // пришли данные регистрации
    void doExit();  // нажата кнопка выхода
    void doChangingStat(QString name, int k);   // пришел сигнал обновления статистики

signals:
    void quit();    // сигнал, посылаемый для выхода

};

#endif // MAINWINDOW_H
