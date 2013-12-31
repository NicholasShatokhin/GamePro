#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <cstdlib>
#include <ctime>
#include <cmath>

class game  // класс игры
{
  int k;  // количество групп
  int* m; // количество предметов в каждой группе

  int k_min,k_max;  // границы генерации
  int m_min,m_max;

  int player;  // кто ходит  (0 - компьютер, 1 - человек, 2 - случайно, 3 -никто)
  int lg;      // вспомогательная переменная
public:
  game();     // конструктор
  game(int k_min_,int k_max_, int m_min_, int m_max_, int first_);  // конструктор
  game(const game& x);  // конструктор копирования
  ~game();      // деструктор
  void set_gen(int k_min_,int k_max_, int m_min_, int m_max_, int first_); // установка параметров генерации
  void gen();  // генерация групп с предметами
  int getk();  // получение количества групп
  int getplayer();  // получение значения переменной player
  void setplayer(int p);    // задание значение переменной player

  int& operator[](int i);  // перегрузка оператора[], доступ к количеству предметов в i группе
  int sum_m();    // XOR сумма количеств предметов групп
  bool is_end();  // проверка, завершена ли игра
  int next(int& count);  // генерация хода компьютера, возвращает номер группы, в которой удалено count предметов
};

#endif // GAMECONTROLLER_H
