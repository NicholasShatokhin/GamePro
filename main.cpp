#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;   // главное окно
    // назначим слот сигналу выхода
    QApplication::connect(&w, SIGNAL(quit()), &a, SLOT(quit()));

    w.show();   // показываем главное окно
    
    return a.exec();    // начинаем работу с приложением
}
