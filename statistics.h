#ifndef STATISTICS_H
#define STATISTICS_H

#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QTextStream>
#include <QWidget>

namespace Ui {
class Statistics;
}
// шаблонный класс
template <typename Templ> class Statistics : public Templ
{    
public:
    explicit Statistics(Templ *parent = 0);
    ~Statistics();

    void paintEvent(QPaintEvent *); // событие перерисовки виджета

    void insertRow(QString name, QString registration, int games, int wins, int loses); // добавление строки в таблицу со статистикой
    void insertRow(QString name, QString registration, QString games, QString wins, QString loses);
    void changeStat(QString name, int k = 0);   // обновление статистики для указанного пользователя
    void loadStat(QString filename);    // загрузка статистики из файла
    void saveStat(QString filename);    // сохранение статистики в файл

    int getCheatingCount();

    bool isStatisticsEmpty();
    bool hasRecord(QString name);

private:
    Ui::Statistics *ui;

    int cheatingCount;

    QString crypt(QString string, int offset);   // шифр Цезаря
};

#endif // STATISTICS_H
