#include <iostream>
#include <iomanip>
#include <vector>

using namespace std;

double h = 1;
const int End = 2;
const double dx1 = 1000;
const double dx2 = 10000;
const double target = 0.001;

double FindRoot();
void DifferentialEquation();
void CalculateApproximation(double);
double CalculateIntegral();
void StartCalculate();

struct Coords
{
	double X;
	double Y;
};

vector<Coords> ptr, ptr2, Approx;

int main()
{
	setlocale(LC_ALL, "Rus");
	system("mode con cols=55 lines=66");

	cout << "\n\t\t>> КУРСОВАЯ РАБОТА <<\n";

	StartCalculate();

	CalculateApproximation(0);

	double Rezult = CalculateIntegral();

	cout << "\n\t-----------------------------------";
	cout << "\n\t4) Количество теплоты:\n\n\tQ = " << Rezult << "\n";

	cout << "\n\tЗАВЕРШИТЬ - Любая клавиша ";
	cin.get();

	return 0;
}

double SetPrec(double value, int dx)
{
	return round(value * dx) / dx;
}

double Poly(double x)
{
	return 3 * pow(x, 4) + 8 * pow(x, 3) + 6 * pow(x, 2) - 10;
}

double DiffUr(double x, double y)
{
	return 1 - sin(3 * x + y) + y / (2 + x);
}

double Calculate(double x, double y)
{
	double K1 = DiffUr(x, y);
	double K2 = DiffUr(x + h / 2.0, y + (h / 2.0) * K1);
	double K3 = DiffUr(x + h / 2.0, y + (h / 2.0) * K2);
	double K4 = DiffUr(x + h, y + h * K3);

	return y + h / 6 * (K1 + 2 * K2 + 2 * K3 + K4);
}

void DifferentialEquation()
{
	for (int val = 0; val < 2; val++)
	{
		double X = 0, Y;
		int CountStep = End / h;

		for(int i = 0; i < CountStep; i++)
		{
			X += h;

			if (val == 0)
			{
				Y = Calculate(ptr[i].X, ptr[i].Y);
				Coords Point{ X, SetPrec(Y, dx2) };
				ptr.push_back(Point);
			}
			else
			{
				Y = Calculate(ptr2[i].X, ptr2[i].Y);
				Coords Point{ X, SetPrec(Y, dx2) };
				ptr2.push_back(Point);
			}
		}
		if(val == 0) h /= 2;
	}	
}

void StartCalculate()
{
	double Y0 = FindRoot();
	double Target = 0.0001 * 15;
	bool GoalPrecision;

	do
	{
		ptr.clear();
		ptr2.clear();

		Coords p{ 0, Y0 };
		ptr.push_back(p);
		ptr2 = ptr;

		DifferentialEquation();

		GoalPrecision = true;

		for (int i = 1; i < ptr.size(); i++)
		{
			if (abs(ptr[i].Y - ptr2[i * 2].Y) > Target)
			{
				GoalPrecision = false;
				break;
			}
		}

	} while (GoalPrecision != true);

	cout << "\n\t-----------------------------------";
	cout << "\n\t2) Решение уравнения, h = " << h << ":\n\n";
	cout << "\tX:" << "\t\tY:" << "\n";

	auto it = ptr2.begin(), nd = ptr2.end();

	for (auto p = it; p != nd; p++)
	{
		cout << '\t' << p->X;
		printf("\t\t%.4f\n", p->Y);
	}
}

double FindRoot()
{
	int i = 1;
	double Root;
	Coords ptr{ 0 , 1 };
	while (true)
	{
		Root = ptr.X - (ptr.Y - ptr.X) / (Poly(ptr.Y) - Poly(ptr.X)) * Poly(ptr.X);

		if (Root - ptr.X < target)
		{
			cout << "\n\t1) Наименьший положительный корень:\n\n\tИтераций - "
				<< i << "\n\tRoot = " << SetPrec(Root, dx1) << endl;

			return SetPrec(Root, dx1);
		}
		ptr.X = Root;
		i++;
	}
}

void CalculateApproximation(double X)
{
	double ApproxStep = 0.1;
	int CountStep = 20;

	for (int i = 0; i <= CountStep; i++)
	{
		for (int j = 1; j < ptr2.size(); j++)
		{
			if (X > ptr2[j - 1].X && X < ptr2[j].X || X == ptr2[j - 1].X)
			{
				double q = (X - ptr2[j - 1].X) / h;
				double DeltaY = ptr2[j].Y - ptr2[j - 1].Y;
				double Y = ptr2[j - 1].Y + q * DeltaY;
				Coords Point{ X, SetPrec(Y, dx2) };
				Approx.push_back(Point);
				break;
			}
			if (int(X) == 2)
			{
				Coords Point{ X, ptr2[ptr2.size()-1].Y };
				Approx.push_back(Point);
				break;
			}
		}
		X += ApproxStep;
	}
	cout << "\n\t-----------------------------------";
	cout << "\n\t3) Результат интерполяции, h = " << ApproxStep << ":\n\n";
	cout << "\tX:" << "\t\tY:" << "\n";

	auto it = Approx.begin(), nd = Approx.end();

	for (auto p = it; p != nd; p++)
	{
		cout << '\t' << p->X;
		printf("\t\t%.4f\n", p->Y);
	}
}

void ToSquareY(int i)
{
	if (i < 0) return;
	Approx[i].Y *= Approx[i].Y;
	ToSquareY(--i);
}

double SummY(int i)
{
	if (i == 0) return 0;
	return Approx[i].Y + SummY(i-1);
}

double CalculateIntegral()
{
	int n = Approx.size() - 1;
	ToSquareY(n);

	return 0.1 * ((Approx[0].Y + Approx[n].Y) / 2 + SummY(n-1));
}


