#include <iostream>
#include <conio.h>
#include <cfloat>
#include <iomanip>
#include <map>
using namespace std;

const int m = 3, n = 6,
          m1 = 5, n1 = 4;

double STable[m1][n1];

double Z[] = { 0,5,1 };
double B[m][n] = { {-4,-1,1,0,0,-9},
                   {-3,-2,0,1,0,-13},
                   {-2,-5,0,0,1,-16} };

int BaseElements = 3;
int CheckSolution = 0;
int NumberTable = 1;
map <int, float> M;

void X0();
void Table();
void Optimal();
void PrintSTable();

int main()
{
    setlocale(LC_ALL, "Rus");
    system("mode con cols=105 lines=45");

    cout << '\n' << setw(60) << ">> КУРСОВАЯ РАБОТА <<";

    X0();
    Table();
    do
    {
        PrintSTable();
        Optimal();
    } while (!CheckSolution);

    cout << "\n\n\n  Любая клавиша - ЗАВЕРШИТЬ ";
    auto Exit = _getch();

    return 0;
}

void Plan()
{
    map <int, float>::iterator it = M.begin();
    cout << "X = (";
    for (; it != M.end(); it++)
        if (it->first != n - 1)cout << it->second << ";";
        else cout << it->second;
    cout << ")";
}

double PrintF(double n)
{
    if (abs(n) - abs((int)n) > 0) return trunc(n * 10) / 10;

    else return n;
}

void Calculate(int Row, int Col)
{
    double Temp = STable[Row][0];
    STable[Row][0] = STable[0][Col];
    STable[0][Col] = Temp;

    for (int i = 1; i < m1; i++)
        if (i != Row)
            for (int j = 1; j < n1; j++)
                if (j != Col)
                {
                    STable[i][j] = STable[i][j] - STable[Row][j] *
                        STable[i][Col] / STable[Row][Col];
                }
    for (int i = 1; i < m1; i++)
        if (i != Row)
            STable[i][Col] = STable[i][Col] / STable[Row][Col] * -1;

    for (int j = 1; j < n1; j++)
        if (j != Col)
            STable[Row][j] = STable[Row][j] / STable[Row][Col];

    STable[Row][Col] = 1 / STable[Row][Col];

    map <int, float>::iterator it = M.begin();
    for (; it != M.end(); it++)
    {
        int Index = 0;
        bool Bool;
        for (int i = 1; i < m1 - 1; i++)
        {
            Bool = it->first == STable[i][0];
            if (Bool)
            {
                Index = i;
                break;
            }
        }
        if (Bool) it->second = STable[Index][1];
        else it->second = 0;
    }
}

void ChoiceElement()
{
    double Min = STable[1][1];
    double CO = 0, Rezult = DBL_MAX;
    int Row = 0, Col = 0;
    for (int i = 1; i < m1 - 1; i++)
    {
        if (STable[i][1] < 0 && STable[i][1] <= Min)
        {
            Min = STable[i][1];
            Row = i;
        }
    }
    cout << setw(8) << "CO" << "      ";
    for (int j = 2; j < n1; j++)
        if (STable[Row][j] < 0)
        {
            CO = abs(STable[m1 - 1][j] / STable[Row][j]);
            if (CO < Rezult)
            {
                Rezult = CO;
                Col = j;
            }
            cout << setw(6) << PrintF(Rezult);
        }
        else
            cout << setw(4) << "-";

    cout << "  => План не оптимален: ";
    Plan();
    cout << ", разрешающий элемент[" << Row << ';' << Col << ']';

    Calculate(Row, Col);
}

void Optimal()
{
    int Optimal = 1;
    for (int i = 1; i < m1 - 1; i++)
        if (STable[i][1] < 0)
            Optimal = 0;
    if (Optimal == 0)
        ChoiceElement();
    else
    {
        cout << setw(29) << " " << "* План оптимален, решение найдено: ";
        CheckSolution = 1;
        Plan();
    }
}

void PrintSTable()
{
    int S = 6;
    cout << "\n\n\n Таблица-" << NumberTable++ << endl;
    for (int i = 0; i < m1; i++)
    {
        if (i == 0) 
            cout << setw(S+8) << "b" << setw(S-1) << "-X" << STable[i][2] 
            << setw(S-1) << "-X" << STable[i][3];
        else if (i == m1 - 1) 
        { 
            cout << setw(S+2) << "Z" << setw(S) << STable[i][1] << setw(S) 
                << STable[i][2] << setw(S) << STable[i][3];
        }
        else
            for (int j = 0; j < n1; j++)
                if (j == 0)
                    cout << setw(S+1) << "X" << STable[i][j];
                else
                    cout << setw(S) << STable[i][j];
        cout << '\n';
    }
}
void Table()
{
    for (int i = 0; i < m1; i++)
    {
        if (i == 0)
            for (int j = 2; j < n1; j++)
                STable[i][j] = j-1;

        else if (i == m1 - 1)
            for (int j = 1; j < n1; j++)
                STable[i][j] = Z[j - 1];

        else {
            STable[i][0] = BaseElements++;
            STable[i][1] = M[STable[i][0]];
            STable[i][2] = B[i - 1][0];
            STable[i][3] = B[i - 1][1];
        }
    }
}
void X0()
{
    int Index;
    for (int i = 0; i < n - 1; i++)
    {
        double CheckSumm = 0;
        for (int j = 0; j < m; j++)
        {
            CheckSumm += abs(B[j][i]);
            if (B[j][i] == 1) Index = j;
        }
        if (CheckSumm == 1)
            M[i + 1] = B[Index][n - 1];
        else
            M[i + 1] = 0;
    }
}
