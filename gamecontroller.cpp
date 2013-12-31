#include "gamecontroller.h"

int game::next(int& count)  // ход компьютера
{
        if (player==0 && !is_end())
        {
                player=1;
                int i=0;
                int sm=sum_m();
                if (sm==0)
                {
                    while (i<k && m[i]==0)
                       i++;
                    count=1;
                }
                else
                {
                    while ( (i<k) && (((m[i])^sm)>=m[i]))
                       i++;
                    count=m[i]-((m[i])^(sm));
                    m[i]=((m[i])^(sm));
                }
                return i;
        }

        return -1;
}

void game::setplayer(int p) // устанавливаем кто ходит первым
{
  if (p>=0 && p<=2)
     player=p;
  else
     player=3;
}

bool game::is_end() // завершена ли игра?
{
   if (k<=0)
     return true;
   for (int i=0; i<k; i++)
     if (m[i]>0)
       return false;
   return true;
}

int game::getk()    // количество столбцов
{
  return k;
}

int game::getplayer()   // кто ходит первым
{
  return player;
}

game::game(const game& x)   // копирующий конструктор
{
   k=x.k;
   if (k>0)
   {
        m=new int [k];
        for (int i=0; i<k; i++)
          m[i]=x.m[i];
   }
   else
     m=NULL;
}

int game::sum_m()   // XOR сумма количеств предметов групп
{
  int s=0;
  for (int i=0; i<k; i++)
    s=s^(m[i]);
  return s;

}

int& game::operator[](int i)    // перегруженный оператор []
{
        if (i>=0 && i<k && m)
           return m[i]; // возвращаем количество фишек в указанном столбце
        return lg;
}

game::~game()   // деструктор
{
  if (m)
    delete [] m;    // освобождаем память, выделенную под количество фишек в столбцах
}
void game::gen()
{   // генерируем случайные количество столбцов
   k=k_min+rand()%(k_max-k_min+1);
   if (m)
     delete [] m;
   // и количество фишек в каждом столбце
   m=new int [k];
   for (int i=0; i<k; i++)
        m[i]=m_min+rand()%(m_max-m_min+1);

}

game::game(int k_min_,int k_max_, int m_min_, int m_max_, int first_move)   // конструктор с параметрами
{   // устанавливаем эти параметры
   srand(time(NULL));
   k=0;
   m=NULL;
   k_min=k_min_;
   k_max=k_max_;
   m_min=m_min_;
   m_max=m_max_;
   player=first_move;
   lg = 0;
}

void game::set_gen(int k_min_,int k_max_, int m_min_, int m_max_, int first_move)
{   // устанавливаем параметры игры
   k_min=k_min_;
   k_max=k_max_;
   m_min=m_min_;
   m_max=m_max_;
   player=first_move;
}

game::game()    // конструктор по умолчанию
{   // устанавливаем параметры по умолчанию
   srand(time(NULL));
   k=0;
   m=NULL;
   k_min=3;
   k_max=10;
   m_min=3;
   m_max=10;
   player=2;
   lg = 0;
}
