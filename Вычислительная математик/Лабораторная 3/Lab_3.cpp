#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

const int CountX = 18;
const double h = 0.04;
const double c = 2;
const double dx1 = 10000;
const double dx2 = 100;

struct Data
{
	double X;
	double Y;
} Array[CountX];

int CalculateFunction(double, int);
int CalculateDerivative(double, int);

int main()
{
	setlocale(LC_ALL, "Rus");
	system("mode con cols=60 lines=51");

	cout << "\n\t\t>> ЛАБОРАТОРНАЯ РАБОТА №3 <<\n\n";
	cout << "\n\tЗначения функции 1/2 * sin2x, h = 0.04:\n\n";
	cout << "\t\tX:" << "\t\tY:" << "\n\n";

	CalculateFunction(c-h, 0);

	cout << "\t----------------------------------------\n";
	cout << "\n\tТаблица значений производных, h = 0.04:\n\n";
	cout << "\tX:" << "\t\tY(приближ):\tY(точные):"<< "\n\n";
	
	CalculateDerivative(c, 0);

	cout << "\n\tЗАВЕРШИТЬ - Любая Клавиша ";
	cin.get();

	return 0;
}

double SetPrecision(double value, int dx)
{
	return round(value * dx) / dx;
}

int CalculateDerivative(double x, int i)
{
	if (i >= CountX-2) return 0;

	double y = (Array[i+2].Y - Array[i].Y) / (2 * h);
		
	double _y = SetPrecision(y, dx2);

	printf("\t%.2f\t\t%.2f\t\t", x, y);
	cout << y << endl;

	return CalculateDerivative(x + h, ++i);
}

int CalculateFunction(double x, int i)
{
	if (i >= CountX) return 0;

	double y = 1. / 2 * sin(c * x);

	Array[i].X = x;
	Array[i].Y = SetPrecision(y, dx1);

	printf("\t\t%.2f\t\t%.4f\n", x, Array[i].Y);

	return CalculateFunction(x + h, ++i);
}


