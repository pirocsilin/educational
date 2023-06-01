#include <iostream>
#include <conio.h>
#include <iomanip>

using namespace std;

const int m = 5, n = 6;

double A[m][n] = { {3,1,5,-2,3,35},   // исходная расширенная матрица
                   {4,3,-7,5,6,54},
                   {-7,5,4,1,-1,-96},
                   {1,4,1,-3,-10,-71},
                   {6,-9,-8,-8,-2,59} };

void PrintMatrix();
void ChoiceElement(int);
void TransformMatrix(int);
void Otvet();

int main()
{
    setlocale(LC_ALL, "");
    system("mode con cols=70 lines=65");

    cout << '\n' << setw(50) << ">> ЛАБОРАТОРНАЯ РАБОТА №1 <<\n\n";

    cout << setw(34) << "Исходная расширенная матрица:\n";
    PrintMatrix();

    for (int k = 0; k < m; k++)         // основной цикл, переход к new матрице
    {
        if (k < m-1)  ChoiceElement(k);

        TransformMatrix(k);

        cout << setw(24) << "Результат итерации №" << k + 1 << ":" << endl;

        PrintMatrix();
    }
    Otvet();
    cout << "\n\n Любая клавиша - ЗАВЕРШИТЬ "; 
    auto Exit = _getch();

    return 0;
}
void Otvet()
{
    cout << " Система имеет единственное решение:" << endl;

    for (int i = 0; i < m; i++)
        printf(" X%d = %d  ", i + 1, (int)round(A[i][5]));
}
void TransformMatrix(int k)
{
    double BaseNumber = A[k][k];
    
    for (int i = 0; i < m; i++)
        for (int j = k + 1; j < n; j++)
            if (i != k)
                A[i][j] = A[i][j] - A[k][j] * A[i][k] / BaseNumber;

    for (int v = k; v < n; v++)
        A[k][v] = A[k][v] / BaseNumber;

    for (int v = 0; v < m; v++)
        if (v != k)
            A[v][k] = 0;
}
void ChoiceElement(int k)               // поиск max элемента в k-м столбце
{
    double max = A[k][k];
    int k_0 = -1;
    for (int i = k; i < m; i++)
    {
        if (abs(A[i][k]) > abs(max))
        {
            max = A[i][k];
            k_0 = i;
        }
    }
    if (k_0 != -1)
    {
        for (int i = k; i < n; i++)
        {
            double temp = A[k][i];
            A[k][i] = A[k_0][i];
            A[k_0][i] = temp;
        }
        cout << setw(17) << "Обмен строк: " << k+1 << " на " << k_0+1 << endl;
        PrintMatrix();
    }
}
void PrintMatrix()                      // вывод матрицы
{
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            double Delta = abs(A[i][j]) - round(abs(A[i][j]));

            if (abs(Delta) > 0.0001)
            {
                printf("%9.3f", A[i][j]);
            }
            else
                cout << setw(9) << A[i][j];
        }
        cout << endl;
    }
    cout << endl;
}
