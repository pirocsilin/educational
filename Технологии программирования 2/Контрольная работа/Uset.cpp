
#include <stdio.h>
#include <iostream>
#include <initializer_list>
#include <set>

using namespace std;

template <typename T>                   // Шаблон класса
class Tset {

private:
    set<T, less<T>> s;                  // Поле класса

public:
    Tset();                             // Конструктор 1    
    Tset(const initializer_list<T>&);   // Конструктор 2    
    void Tclear();                      // Опустошить       
    void Tinsert(T);                    // Добавить         
    void Terase(T);                     // Удалить          
    bool Tempty();                      // Пусто            
    bool Tfind(T) const;                // Принадлежит      
    int Tsize();                        // Элементов        
    T GetElem(int);                     // Элемент          
    void OutPut();                      // Вывести множество

    Tset<T> operator * (const Tset<T>&) const;       // Умножить
    Tset<T> operator + (const Tset<T>&) const;       // Объединить
    Tset<T> operator - (const Tset<T>&) const;       // Вычесть
    bool operator   == (const Tset<T>&) const;       // Сравнить1
    bool operator   != (const Tset<T>&) const;       // Сравнить2
};

// функции-члены, определенные вне шаблона класса
// определяются как шаблоны функций:

template <typename T>
Tset<T>::Tset() { s = set<T, less<T>>(); }

template <typename T>
Tset<T>::Tset(const initializer_list<T>& list) : Tset() {
    for (auto val : list) {
        s.insert(val);
    }
}

template <typename T>
void Tset<T>::Tclear() { s.clear(); }

template <typename T>
void Tset<T>::Tinsert(T n) { s.insert(n); }

template <typename T>
void Tset<T>::Terase(T n) { s.erase(n); }

template <typename T>
bool Tset<T>::Tempty() { return s.empty(); }

template <typename T>
bool Tset<T>::Tfind(T n) const{
    return s.find(n) == s.end() ? false : true;
}

template <typename T>
int Tset<T>::Tsize() { return s.size(); }

template <typename T>
T Tset<T>::GetElem(int i) {

    if (i < 1 || i > s.size()) {
        throw "ERROR Index out of range!";
    }
    auto it = s.begin();
    while (--i > 0)
        it++;
    return *it;
}

template <typename T>
void Tset<T>::OutPut() {

    for (auto it = s.begin(); it != s.end(); it++)
        cout << " " << *it;
    cout << endl;
}

template <typename T>
Tset<T> Tset<T>::operator + (const Tset<T>& q) const {
    Tset<T> tmp = *this;
    tmp.s.insert(q.s.begin(), q.s.end());
    return  tmp;
}

template <typename T>
Tset<T> Tset<T>::operator - (const Tset<T>& q) const {
    Tset<T> tmp = *this;
    for (auto it = q.s.begin(); it != q.s.end(); it++) {
        if (tmp.Tfind(*it))
            tmp.Terase(*it);
    }
    return tmp;
}

template <typename T>
Tset<T> Tset<T>::operator * (const Tset<T>& q) const {
    Tset<T> tmp = Tset<T>();
    for (auto it = q.s.begin(); it != q.s.end(); it++) {
        if ((*this).Tfind(*it))
            tmp.Tinsert(*it);
    }
    return tmp;
}
template <typename T>
bool Tset<T>::operator == (const Tset<T>& q) const {
    return this->s == q.s;
}

template <typename T>
bool Tset<T>::operator != (const Tset<T>& q) const {
    return this->s != q.s;
}