#include "statistics.h"
#include "ui_statistics.h"

template <typename Templ>
Statistics<Templ>::Statistics(Templ *parent) :
    Templ(parent, Qt::Window),
    ui(new Ui::Statistics)
{
    ui->setupUi(this);

    cheatingCount = 0;

    // задаем шапку таблицы
    // создаем массив подписей столбцов
    QStringList labels;
    labels.push_back(this->tr("Имя"));
    labels.push_back(this->tr("Регистрация"));
    labels.push_back(this->tr("Игры"));
    labels.push_back(this->tr("Победы"));
    labels.push_back(this->tr("Поражения"));
    // и устанавливаем в качестве шапки
    ui->table->setHorizontalHeaderLabels(labels);
    // при нажатии на кнопку закрытия закрываем окно
    this->connect(ui->closeBtn, SIGNAL(clicked()), this, SLOT(close()));
}

template <typename Templ>
Statistics<Templ>::~Statistics()
{
    delete ui;
}

template <typename Templ>
void Statistics<Templ>::paintEvent(QPaintEvent * ev)
{
    QPixmap pixmap(":/background.jpg"); // загружаем фоновую картинку
    QPainter painter(this); // указываем пеинтеру рисовать на этом окне

    painter.drawPixmap(0, 0, this->width(), this->height(), pixmap);    // рисуем фоновое изображение

    Templ::paintEvent(ev);  // рисуем все остальное
}

template <typename Templ>
void Statistics<Templ>::insertRow(QString name, QString registration, int games, int wins, int loses)
{
    const int currentRow = ui->table->rowCount();
    ui->table->insertRow(currentRow);   // добавляем в таблицу новую строку
    // и пишем данные в ячейки этой строки
    ui->table->setItem(currentRow, 0, new QTableWidgetItem(name));
    ui->table->setItem(currentRow, 1, new QTableWidgetItem(registration));
    ui->table->setItem(currentRow, 2, new QTableWidgetItem(QString::number(games)));
    ui->table->setItem(currentRow, 3, new QTableWidgetItem(QString::number(wins)));
    ui->table->setItem(currentRow, 4, new QTableWidgetItem(QString::number(loses)));
}

template <typename Templ>
void Statistics<Templ>::insertRow(QString name, QString registration, QString games, QString wins, QString loses)
{
    const int currentRow = ui->table->rowCount();
    ui->table->insertRow(currentRow);   // добавляем в таблицу новую строку
    // и пишем данные в ячейки этой строки
    ui->table->setItem(currentRow, 0, new QTableWidgetItem(name));
    ui->table->setItem(currentRow, 1, new QTableWidgetItem(registration));
    ui->table->setItem(currentRow, 2, new QTableWidgetItem(games));
    ui->table->setItem(currentRow, 3, new QTableWidgetItem(wins));
    ui->table->setItem(currentRow, 4, new QTableWidgetItem(loses));
}

template <typename Templ>
void Statistics<Templ>::changeStat(QString name, int k)
{
    // ищем игрока с указанным именем
    for(int i=0;i<ui->table->rowCount();i++)
    {
        if(ui->table->item(i, 0)->text().compare(name) == 0)    // если игрок найден
        {
            // то увеличиваем ему количество игр
            ui->table->item(i, 2)->setText(QString::number(ui->table->item(i, 2)->text().toInt() + 1));
            // и увеличием количество побед или поражений
            if(k > 0)
                ui->table->item(i, 3)->setText(QString::number(ui->table->item(i, 3)->text().toInt() + 1));
            else if(k < 0)
                ui->table->item(i, 4)->setText(QString::number(ui->table->item(i, 4)->text().toInt() + 1));

            break;  // выходим из цикла
        }
    }
}

template <typename Templ>
void Statistics<Templ>::loadStat(QString filename)  // загрузка данных
{
    for(int i=0;i<ui->table->rowCount();i++)    // сперва очищаем таблицу
        ui->table->removeRow(i);

    QList<QString> record1, record2;
    QString tmp;

    cheatingCount = 0;

    QFile file(filename);
    // открываем файл
    if(!file.open(QIODevice::ReadWrite))  // если ошибка, покажем сообщение
    {
        QMessageBox(QMessageBox::Critical, this->tr("Ошибка"), this->tr("Файл статистики не существует и его не удалось создать")).exec();

        return; // и прекратим загрузку
    }

    QTextStream stream( &file );

    while(!stream.atEnd())  // пока данные в файле есть
    {   // считываем их
        for(int i=0;i<5;i++)
        {
            tmp = stream.readLine();

            record1.push_back(crypt(tmp, -1));
        }

        for(int i=0;i<5;i++)
        {
            tmp = stream.readLine();

            record2.push_back(crypt(tmp, -2));
        }

        // добавляем в таблицу строку
        const int currentRow = ui->table->rowCount();
        ui->table->insertRow(currentRow);

        for(int i=0;i<5;i++)
        {
            if(record1[i].compare(record2[i]) != 0)
                cheatingCount++;    // увеличиваем количество измененных данных

            ui->table->setItem(currentRow, i, new QTableWidgetItem(record1[i]));    // добавляем ячейку в таблицу
        }

        record1.clear();
        record2.clear();
    }

    file.close();   // закрываем файл
}

template <typename Templ>
void Statistics<Templ>::saveStat(QString filename)  // сохранение данных
{
    QFile file(filename);
    // пробуем открыть файл. Если его нет - создаем, если есть - удаляем содержимое
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))  // если ошибка
    {   // то покажем сообщение
        QMessageBox(QMessageBox::Critical, this->tr("Ошибка"), this->tr("Не удалось создать файл статистики")).exec();
        return; // и выйдем
    }

    QTextStream stream( &file );

    for(int i=0;i<ui->table->rowCount();i++)    // отправляем данные каждой строки в поток
    {
        stream << crypt(ui->table->item(i, 0)->text(), 1) << "\n";
        stream << crypt(ui->table->item(i, 1)->text(), 1) << "\n";
        stream << crypt(ui->table->item(i, 2)->text(), 1) << "\n";
        stream << crypt(ui->table->item(i, 3)->text(), 1) << "\n";
        stream << crypt(ui->table->item(i, 4)->text(), 1) << "\n";

        stream << crypt(ui->table->item(i, 0)->text(), 2) << "\n";
        stream << crypt(ui->table->item(i, 1)->text(), 2) << "\n";
        stream << crypt(ui->table->item(i, 2)->text(), 2) << "\n";
        stream << crypt(ui->table->item(i, 3)->text(), 2) << "\n";
        stream << crypt(ui->table->item(i, 4)->text(), 2) << "\n";
    }

    file.close();   // сохраняем и закрываем файл
}

template <typename Templ>
int Statistics<Templ>::getCheatingCount()
{
    return cheatingCount;
}

template <typename Templ>
bool Statistics<Templ>::isStatisticsEmpty()
{
    return (ui->table->rowCount() > 0);
}

template <typename Templ>
bool Statistics<Templ>::hasRecord(QString name)
{
    for(int i=0;i<ui->table->rowCount();i++)
    {
        if(ui->table->item(i, 0)->text().compare(name) == 0)
        {
            return true;
        }
    }

    return false;
}

template <typename Templ>
QString Statistics<Templ>::crypt(QString string, int offset)
{
    QByteArray temp;
    temp.append(string);

    for(int i=0;i<temp.length();i++)
    {
        temp[i] = temp[i] + offset;
    }

    return QString(temp);
}
