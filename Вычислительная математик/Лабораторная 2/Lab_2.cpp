#include <iostream>
#include <string>
#include <stdio.h>
#include <cmath>
#include <math.h>
using namespace std;

double MultiplyCX(int);
double FindPrecision();
int RecordPreviosX(int);
double SetPrecision(double);
int Rezult(int);

const int n = 4;
const double dx = 100000;
const double target = 0.0001;
const double mu = 0.74;
const double C[n][n] = {{0, 0.2813, -0.1667, 0.2917},
                        {-0.1102, 0, 0.2913, 0.3386},
                        {-0.1543, 0.3029, 0, -0.1829},
                        {0.2429, -0.1695, 0.435, 0}};
const double B[n] = { 2.5833, -2.4882, -0.8686, 0.7288 };
double X[n] = { 2.5833, -2.4882, -0.8686, 0.7288 };
double previosX[n] = { 2.5833, -2.4882, -0.8686, 0.7288 };

int main()
{
    setlocale(LC_ALL, "Rus");
    system("mode con cols=60 lines=78");
    cout << "\n\t\t>> ЛАБОРАТОРНАЯ РАБОТА №2 <<\n\n";

    int Iteration = 1;
    while (true)
    {
        cout << "\tИтерация " << Iteration << ":\n";
        for (int i = 0; i < n; i++)
        {
            X[i] = B[i] - SetPrecision(MultiplyCX(i));

            printf("\t\tX%d = %.5f\n", i + 1, X[i]);
        }

        if (FindPrecision() < target)
        {
            printf("\t\tТочность: %.5f\n", SetPrecision(FindPrecision()));

            cout << " ------------------------------------------\n" << endl;
            cout << " Приближенное решение с точностью до 0.0001:\n" << endl;
            Rezult(0);

            break;
        }
        printf("\t\tТочность: %.5f\n", SetPrecision(FindPrecision()));

        Iteration++;
        RecordPreviosX(n - 1);

        cout << endl;
    }
    cout << "\n\t\tЗАВЕРШИТЬ - Любая Клавиша ";
    cin.get();

    return 0;
}

int Rezult(int i)
{
    if (i >= n) return 0;

    X[i] = round(X[i] * 10000) / 10000;

    printf("\t\tX%d = %.4f\n", i + 1, X[i]);

    return Rezult(++i);
}

double SetPrecision(double val)
{
    return round(val * dx) / dx;
}

double FindPrecision()
{
    double Max = 0;
    for (int i = 0; i < n; i++)
    {
        double temp = abs(X[i] - previosX[i]);
        if (temp > Max) Max = temp;
    }

    return (mu / (1 - mu)) * Max;
}

int RecordPreviosX(int i)
{
    if (i < 0) return 0;

    previosX[i] = X[i];

    return RecordPreviosX(i - 1);
}

double MultiplyCX(int m)
{
    double summ = 0;
    for (int i = 0; i < n; i++)
        summ += C[m][i] * X[i];
    return summ;
}