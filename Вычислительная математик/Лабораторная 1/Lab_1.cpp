#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

struct Data
{
	double X;
	double Y;
	double DeltaY;
} New_Data;

struct Approximation
{
	double X;
	double Y;
} New_Approximation;

const double h = 0.01;
const double c = 2;
double dx = 10000;
const int CountX =  16;

Data* Array = new Data[CountX];
Approximation* Array_2 = new Approximation[CountX-2];

void CalculateFunction(double);
void CalculateApproximation(double);
double Rounding(double);

int main()
{
	setlocale(LC_ALL, "Rus");
	system("mode con cols=60 lines=50");

	cout << "\n\t\t>> ЛАБОРАТОРНАЯ РАБОТА №1 <<\n\n";
	cout << "\n\tЗначения функции 8*cos(x/2 + 10), h = 0.01:\n\n";
	cout << "\t\tX:" << "\t\tY:" << "\n\n";

	CalculateFunction(c);
	
	for (int i = 0; i < CountX; i++)
	{
		cout << "\t\t" << Array[i].X; 
		printf("\t\t%.4f\n", Array[i].Y);
	}

	cout << "\t----------------------------------------\n";
	cout << "\n\tТаблица промежуточных значений, h = 0.006:\n\n";
	cout << "\tX:" << "\t\tY(точные):\tY(приближ):"<< "\n";

	CalculateApproximation(c);
	
	for (int i = 0; i < CountX - 2; i++)
		cout << '\t' << Array_2[i].X << "\t\t" << Array_2[i].Y 
			<< "\t\t" << Rounding(Array_2[i].Y) << endl;

	cout << "\n\tЗАВЕРШИТЬ - Любая клавиша ";
	cin.get();

	return 0;
}

void CalculateApproximation(double x)
{
	for (int i = 0; i < CountX - 2; i++)
	{
		x = c + 0.6 * h * (i + 1.0);

		for (int j = 1; j < CountX; j++)
			if (x > Array[j - 1].X && x < Array[j].X || x == Array[j - 1].X)
			{
				double q = (x - Array[j - 1].X) / h;

				New_Approximation.Y = Array[j - 1].Y + q * Array[j - 1].DeltaY;
				New_Approximation.X = x;

				Array_2[i] = New_Approximation;

				break;
			}
	}
	cout << endl;
}

void CalculateFunction(double x)
{
	for (int i = 0; i < CountX; i++)
	{
		double y = 8 * cos(x / 2 + 10);

		New_Data.X = x;
		New_Data.Y = Rounding(y);
		if (i != 0) 
			Array[i - 1].DeltaY = Rounding(y) - Array[i-1].Y;

		Array[i] = New_Data;

		x += h;
	}
}

double Rounding(double value)
{
	return floor((value * dx) + 0.5) / dx;
}