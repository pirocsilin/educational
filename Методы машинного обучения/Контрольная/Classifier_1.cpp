#include <iostream>
#include <iomanip>
#include <Windows.h>
#include <vector>
#include <algorithm>        // sort

using namespace std;

double p[2]{};
const int k = 3;

struct Rule {
    int x, y, Y;

    Rule(int a, int b, int c = 0) : x(a), y(b), Y(c){}

    bool operator < (Rule& op2) const {
        return sqrt(pow(p[0] - this->x, 2) + pow(p[1] - this->y, 2)) <
               sqrt(pow(p[0] - op2.x, 2)   + pow(p[1] - op2.y, 2));
    }
};

vector<Rule>Arr = { Rule(7,9,1), Rule(2,5,1), Rule(5,6,1), Rule(8,6,1), 
                    Rule(7,6,1), Rule(7,9,2), Rule(14,7,2), Rule(14,2,2), 
                    Rule(6,7,2), Rule(10,3,2), Rule(11,9,2), Rule(9,1,2) };

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=90 lines=170");

    char ch;
    cout << "\n\tВведите точку в формате x,y: ";
    cin >> p[0] >> ch >> p[1];

    sort(Arr.begin(), Arr.end());   // Сортируем точки относительно контрольной

    cout << "\n\tКонтрольная точка: (" << p[0] << ", " << p[1] << ")\n\n";
    cout << "\tТочка:\tУдаленность:\tКласс:\n\n";

    int ClassOne = 0, ClassTwo = 0, Finall;
    for (int i = 0; i < Arr.size(); i++) {

        if (i < k) {                                    // Принадлежность точки классу:
            Arr[i].Y == 1 ? ClassOne++ : ClassTwo++;    // проверка трех "соседей"
        }

        cout << "\t(" <<Arr[i].x << ", " << Arr[i].y << ")\t  "
             << setprecision(3) << setiosflags(ios::showpoint)
             << sqrt(pow(p[0] - Arr[i].x, 2) + pow(p[1] - Arr[i].y, 2))
             << "\t\t" << Arr[i].Y << endl;
    }

    ClassOne > ClassTwo ? Finall = 1 : Finall = 2;
    cout << "\n\tКонтролная точка имеет класс: " << Finall << endl << endl;
    system("pause");

    return 0;
}