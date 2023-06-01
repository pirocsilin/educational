#include <iostream>
#include <conio.h>
#include <iomanip>
#include <map>

using namespace std;

const int m = 100, n = 7;

double GameRez[m][n];

int MatrixPrice[2][2] = { {9,13},
                          {21,8} };

map <float, string> Strategies()
{
    map <float, string> M;
    M[1.1] = "A1";
    M[1.2] = "A2";
    M[2.1] = "B1";
    M[2.2] = "B2";

    return M;
}

void StartGame();
void PrintMatrix();
void Frequencys();

int main()
{
    setlocale(LC_ALL, "");
    system("mode con cols=105 lines=115");

    cout << '\n' << setw(70) << ">> ЛАБОРАТОРНАЯ РАБОТА №2 <<\n\n";

    StartGame();
    PrintMatrix();
    Frequencys();

    cout << "\n\n Любая клавиша - ЗАВЕРШИТЬ ";
    auto Exit = _getch();

    return 0;
}

void StartGame()
{
    double sum = 0;
    int k, v;

    for (int i = 0; i < m; i++)
    {
        GameRez[i][0] = (double)rand() / RAND_MAX;
        if (GameRez[i][0] < float(13) / 17) 
        {
            k = 0; 
            GameRez[i][1] = 1.1;
        }
        else 
        {
            k = 1; 
            GameRez[i][1] = 1.2;
        }

        GameRez[i][2] = (double)rand() / RAND_MAX;
        if (GameRez[i][2] < float(12) / 17) 
        {
            v = 0; 
            GameRez[i][3] = 2.2;
        }
        else 
        {
            v = 1; 
            GameRez[i][3] = 2.1;
        }

        GameRez[i][4] = MatrixPrice[k][v];
        sum += GameRez[i][4];
        GameRez[i][5] = sum;
        GameRez[i][6] = sum / (i + 1);
    }
}

void Frequencys()
{
    cout << " Относительные частоты:\n ";
    int a = 0, b = 0;
    for (int i = 0; i < m; i++)
    {
        if (GameRez[i][1] == 1.1) a += 1;
        if (GameRez[i][3] == 2.1) b += 1;
    }
    printf("p=(%d/%d;%d/%d), q=(%d/%d;%d/%d)", a, m, m - a, m, b, m, m - b, m);
}

void PrintMatrix()
{
    map <float, string> M = Strategies();

    cout << " Номер  |" << " Случайное число |" << " Стратегия |" << " Случайное число |" 
        << " Стратегия |" << " Выигрыш  |" << " Накопленный |" << " Средний   " << endl;

    cout << " партии |" << " для игрока А    |" << " игрока А  |" << " для игрока В    |" 
        << " игрока В  |" << " игрока А |" << " выигрыш А   |" << " выигрыш А \n" << endl;

    for (int i = 0; i < m; i++)
    {
        cout << setw(5) << i + 1 << '.';

        for (int j = 0; j < n; j++)
        {
            if (j == 1 || j == 3)
                cout << setw(13) << M[GameRez[i][j]];

            else if (j == 4 || j == 5)
                cout << setw(12) << GameRez[i][j];

            else
                printf("%15.3f", GameRez[i][j]);
        }
        cout << endl;
    }
    cout << endl;
}
