#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    entr = false;

    login = new Login(this);    // создаем виджет входа в игру
    ui->menuBar->hide();    // скроем меню
    ui->menuBar->setEnabled(false); // отключим меню (нужно для Ubuntu, т.к. там оно не скрывается)
    // назначим слоты сигналам виджета
    connect(login, SIGNAL(doLogin(QString,QString)), this, SLOT(doLogin(QString,QString)));
    connect(login, SIGNAL(doRegistration()), this, SLOT(doRegistration()));
    connect(login, SIGNAL(doExit()), this, SLOT(doExit()));
    // слот сигналу выбора пункта "Выход" в меню
    connect(ui->exitMenu, SIGNAL(triggered()), this, SLOT(doExit()));

    registration = new Registration(this);  // создаем виджет регистрации
    // назначаем слоты сигналу виджета
    connect(registration, SIGNAL(doRegistration(QString,QString)), this, SLOT(doRegistration(QString,QString)));

    game = new Game(this);  // создаем виджет игрового поля
    game->hide();   // и сразу его скрываем
    // назначим слот сигналу выбора пункта меню "Новая игра"
    connect(ui->newGameMenu, SIGNAL(triggered()), game, SLOT(startGame()));

    statistics = new Statistics<QWidget>(this); // создаем виджет статистики
    statistics->loadStat("stat.txt");   // загружаем статистику из файла stat.txt

    connect(ui->statisticsMenu, SIGNAL(triggered()), statistics, SLOT(show())); // назначим слот сигналу выбора пункта меню "Статистика"
    connect(game, SIGNAL(updateStat(QString,int)), this, SLOT(doChangingStat(QString,int)));    // слот сигналу обновить статистику

    options = new Options(this);    // создаем виджет настроек

    connect(ui->settingsMenu, SIGNAL(triggered()), options, SLOT(show()));  // слот сигналу выбора пункта меню "Настройки"
    connect(options, SIGNAL(saveParams(int,int,int,int,int)), game, SLOT(setParams(int,int,int,int,int)));  // слот сигналу с параметрами настроек

    options->emitParams();  // заставим виджет послать сигнал с параметрами настроек
}

MainWindow::~MainWindow()
{   // удаляем виджеты
    delete login;
    delete registration;
    delete game;
    delete statistics;
    delete options;

    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *)    // если игрок меняет размеры окна
{   // то изменим размеры виджетов в этом окне
    login->resize(size());
    game->move(0, ui->menuBar->height());
    game->resize(width(), height()-ui->menuBar->height());
}

void MainWindow::closeEvent(QCloseEvent *)  // если игрок закрыл окно
{
    statistics->saveStat("stat.txt");   // сохраняем статистику в файл stat.txt
}

void MainWindow::switchToGame(QString name) // переключаемся на виджет с игровым полем
{
    login->hide();  // скрываем виджет входа
    game->show();   // показываем виджет с игрой
    game->setName(name);    // устанавливаем имя игрока
    ui->menuBar->show();    // покажем меню
    ui->menuBar->setEnabled(true);  // включим меню
    game->move(0, ui->menuBar->height());                   // установим размеры
    game->resize(width(), height()-ui->menuBar->height());  // виджета с игрой
}

void MainWindow::doLogin(QString name, QString password)    // произвести вход в игру
{
    QFile file("sys.game");
    if (!file.open(QIODevice::ReadOnly))    // открываем файл с данными игрока
    {   // если не удалось, то выведем ошибку
        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Ошибка открытия системного файла игры.")).exec();
    }
    else
    {
            pwd w;
            file.read((char*)&w,sizeof(pwd));   // читаем первую запись - в ней сохранено количество записей в файле
            int n=w.id;
            if (n>0)    // если записи есть
            {
               for (int i=0; i<n; i++)  // то поочереди считываем их
               {
                  file.read((char*)&w,sizeof(pwd));
                  if (password.compare(QString(w.pass)) == 0 && name.compare(QString(w.login)) == 0)    // если это запись, соответствующая введенному логину и пароли совпадают
                  {
                      if(statistics->getCheatingCount() > 0)
                          QMessageBox(QMessageBox::Warning, tr("Внимание!"), tr(QString("Обнаружено %1 изменений в файле статистики").arg(statistics->getCheatingCount()).toStdString().c_str())).exec();

                      if(!statistics->hasRecord(name))
                      {
                          QDateTime date = QDateTime::currentDateTime();  // текущая дата
                          statistics->insertRow(name, date.toString("dd.MM.yyyy"), 0, 0, 0);
                      }

                      // вход
                      entr=true;
                      file.close(); // закрываем файл

                      switchToGame(name);   // и переходим к игре
                      return;   //выходим из функции
                  }
               }
            }
            // иначе данные введены неверно и выводим сообщение об ошибке
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Данные для входа указаны неверно"));
            file.close();   // и закрываем файл
    }
}

void MainWindow::doRegistration()
{
    registration->show();   // показываем окно решистрации
}

void MainWindow::doRegistration(QString name, QString password)
{
    if (!QFile::exists("sys.game"))   // файла нет
    {
        QFile out("sys.game");
        if (!out.open(QIODevice::WriteOnly))    // если не получилось создать системный файл, то выведем ошибку
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось создать системный файл игры")).exec();
        else
        {
            pwd w;
            w.id=1;
            out.write((const char*)&w, sizeof(pwd));    // пишем количество записей в файл

            w.id=1; // сохраняем регистрационные данные в спомогательную структуру
            strcpy(w.pass,password.toStdString().c_str());
            strcpy(w.login,name.toStdString().c_str());
            out.write((const char*)&w, sizeof(pwd));    // и пишем ее в файл
            out.close();    // сохраняем и закрываем файл

            QDateTime date = QDateTime::currentDateTime();  // текущая дата
            statistics->insertRow(name, date.toString("dd.MM.yyyy"), 0, 0, 0);  // добавляем строку в таблицу статистики
            // и выводим сообщение
            QMessageBox(QMessageBox::Information, tr("Успех"), tr("Регистрация прошла успешно")).exec();

            registration->close();  // закрываем окно регистрации
            registration->clearForm();   // и очищаем его поля
        }
    }
    else // файл есть
    {
        QFile in("sys.game");
        if(!in.open(QIODevice::ReadOnly))   // если не удалось открыть файл, то выведем ошибку
            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось открыть системный файл игры")).exec();
        else
        {
            pwd w;
            in.read((char*)&w,sizeof(pwd)); // считываем количество записей
            if (in.atEnd())  // не удалось прочитать
            {   // ошиба
                QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось прочитать данные из системного файла игры")).exec();
                in.close(); // закроем файл
            }
            else  // удалось прочитать
            {
                int n=w.id;
                pwd* a;
                if (n>0)// если есть записи
                {
                    a=new pwd [n];
                    bool fl=false;
                    for (int i=0; i<n; i++) // проходим по всем записям
                    {
                        in.read((char*)&(a[i]),sizeof(pwd)); // считываем каждую
                        if (name.compare(QString(a[i].login)) == 0) // если имя в записи такое же, как введенное
                        {
                            fl=true;    // запись существует
                            break;  // выходим из цикла
                        }
                    }
                    in.close(); // закрываем файл

                    if (fl) // если такой пользователь уже есть
                    {   // выводим сообщение об ошибке
                        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Указанное имя уже зарегистрировано.")).exec();
                        delete [] a;
                    }
                    else    // иначе
                    {
                        w.id++; // увеличиваем количество записей в файле
                        QFile out("sys.game");
                        if (!out.open(QIODevice::WriteOnly))    // если не удалось открыть файл на запись, то выведем ошибку
                            QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось создать системный файл игры")).exec();
                        else
                        {
                            out.write((const char*)&w, sizeof(pwd));    // пишем в файл количество записей

                            for (int i=0; i<n; i++)
                                out.write((const char*)&(a[i]), sizeof(pwd));   // сохраняем записи в файл
                            w.id=1; // создаем запись для нового пользователя
                            strcpy(w.pass,password.toStdString().c_str());
                            strcpy(w.login,name.toStdString().c_str());
                            out.write((const char*)&w, sizeof(pwd));    // сохраняем ее в файл
                            out.close();    // сохраняем и закрываем файл


                            QDateTime date = QDateTime::currentDateTime();  // текущая дата
                            statistics->insertRow(name, date.toString("dd.MM.yyyy"), 0, 0, 0);  // добавим запись в статистику
                            // выведем сообщение
                            QMessageBox(QMessageBox::Information, tr("Успех"), tr("Регистрация прошла успешно")).exec();

                            registration->close();  // закроем окно регистрации
                            registration->clearForm();  // и очистим его поля
                        }
                    }
                }
                else  // если нет записей
                {
                    in.close();
                    QFile out("sys.game");
                    if (!out.open(QIODevice::WriteOnly))    // если не удалось открыть файл на запись, то выведем ошибку
                        QMessageBox(QMessageBox::Critical, tr("Ошибка"), tr("Не удалось создать системный файл игры")).exec();
                    else
                    {
                        w.id=1;
                        out.write((const char*)&w, sizeof(pwd));    // сохраняем количество записей


                        strcpy(w.pass,password.toStdString().c_str());
                        strcpy(w.login,name.toStdString().c_str());
                        out.write((const char*)&w, sizeof(pwd));    // сохраняем новую запись
                        out.close();    // сохраняем и закрываем файл

                        QDateTime date = QDateTime::currentDateTime();  // текущая дата
                        statistics->insertRow(name, date.toString("dd.MM.yyyy"), 0, 0, 0);  // добавляем строку в таблицу статистики
                        // выводим сообщение
                        QMessageBox(QMessageBox::Information, tr("Успех"), tr("Регистрация прошла успешно")).exec();

                        registration->close();  // закрываем окно регистрации
                        registration->clearForm();   // и очищаем его поля
                    }

                }
            }
        }
    }

}

void MainWindow::doExit()   // выход из игры
{
    statistics->saveStat("stat.txt");   // сохраняем статистику
    emit quit();    // сигнал выхода
}

void MainWindow::doChangingStat(QString name, int k)
{
    statistics->changeStat(name, k);    // изменяем статистику для указанного пользователя
}
