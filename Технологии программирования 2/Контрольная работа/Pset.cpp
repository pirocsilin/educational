
#include "Uset.cpp"
#include <windows.h>

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    cout << "\n Тестирование класса Tset:\n" << endl;
    Tset<int> s1 = Tset<int>();

    s1.Tinsert(777);
    s1.Tinsert(12);
    s1.Tinsert(34);
    s1.Tinsert(5);

    Tset<int> s2 = Tset<int>({ 777, 12, 27, -4 });

    Tset<int> s3 = s1 + s2;
    Tset<int> s4 = s1 - s2;

    cout << " s1      = ";    s1.OutPut();
    cout << " s2      = ";    s2.OutPut();
    cout << " s1 != s2   " << (s1 != s2) << endl; 
    cout << " s1 + s2 = ";    s3.OutPut();
    cout << " s1 - s2 = ";    s4.OutPut();
    cout << " s1 * s2 = ";    (s1 * s2).OutPut();

    cout << endl;
    try {
        cout << " Получить 2-й элемент множества s1: ";
        cout << s1.GetElem(2) << endl;
        cout << " Получить 8-й элемент множества s1: ";
        cout << s1.GetElem(8) << endl;
    }
    catch (const char* msg) {
        cout << msg;
    }
    cout << endl << endl;
    system("pause");

    return 0;
}

