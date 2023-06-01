#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <deque>

using namespace std;

int GetNOD(int a, int b) {
	while (a != b) {
		while (a > b) { a -= b; }
		while (b > a) { b -= a; }
	}
	return a;
}

class TFract {
	friend int GetNOD(int, int);
private:
	int num, denom;
public:
	class ErrFract {
	public:
		const char* msg;
		ErrFract(const char* str) : msg(str) {}
	};

	TFract() : num(0), denom(1) {}

	TFract(int a, int b) {

		if (b == 0) throw ErrFract("Ошибка инициализации");

		if ((a <= 0 && b < 0) || (a > 0 && b < 0))
		{
			a *= -1; b *= -1;
		}

		if (a == 0) { num = 0; denom = b; }

		else
		{
			int nod = GetNOD(abs(a), abs(b));
			num = a / nod;
			denom = b / nod;
		}
	}
	TFract(string ab) {
		char ch;
		istringstream is(ab);
		is >> num >> ch >> denom;
		*this = TFract(num, denom);
	}
	string Display() {
		ostringstream os;
		os << num << "/" << denom;
		return os.str();
	}
	TFract GetCopy() {

		return TFract(num, denom);
	}
	TFract GetSqr() {

		return TFract(num * num, denom * denom);
	}
	double GetNum() {
		return num;
	}
	double GetDenom() {
		return denom;
	}
	string GetNumStr() {
		return to_string(num);
	}
	string GetDenomStr() {
		return to_string(denom);
	}
	TFract operator + (const TFract& op2) const {

		int a = num * op2.denom + denom * op2.num;
		int b = denom * op2.denom;

		return TFract(a, b);
	}
	TFract operator * (const TFract& op2) const {

		int a = num * op2.num;
		int b = denom * op2.denom;

		return TFract(a, b);
	}
	TFract operator - (const TFract& op2) const {

		int a = num * op2.denom - denom * op2.num;
		int b = denom * op2.denom;

		return TFract(a, b);
	}
	TFract operator - () const {

		return TFract(num * -1, denom);
	}
	TFract operator / (const TFract& op2) const {

		if (denom == 0 || op2.num == 0)
			throw ErrFract("Делитель равен нулю");

		int sign = op2.num < 0 ? -1 : 1;

		TFract b = TFract(op2.denom * sign, op2.num * sign);

		return *this * b;
	}
	bool operator == (const TFract& op2) const {

		return num * op2.denom - denom * op2.num == 0;
	}
	bool operator > (const TFract& op2) const {

		return (*this - op2).num > 0;
	}
	bool operator < (const TFract& op2) const {

		return (*this - op2).num < 0;
	}
};

TFract InputUserFract() {
	const char* err = "Ошибка ввода данных";
	TFract UserFract;
	bool wrong = true;
	string rez;
	int a, b;
	char ch;

	do {
		getline(cin, rez);

		try {
			istringstream is(rez);
			is >> a >> ch >> b;
			if (ch != '/') {
				throw err;
			}

			UserFract = TFract(a, b);
			wrong = false;
		}
		catch (TFract::ErrFract e) {
			cout << " ERROR: [" << e.msg << "]" << endl;
			cout << "\tПовторный ввод: ";
		}
		catch (const char*) {
			cout << " ERROR: [" << err << "]" << endl;
			cout << "\tПовторный ввод: ";
		}
	} while (wrong);

	return UserFract;
}

deque<TFract> d(5);

void PrintDeque() {
	for (auto it = d.begin(); it != d.end(); it++) {
		cout << '\t' << it->Display();
	};
}

class InOutDo
{
public:
	static void Input()
	{
		cout << "\n Введи простую дробь (пример 2/3) и нажмите Enter:\n" << endl;

		for (int i = 0; i < 5; i++) {
			cout << "\t" << i + 1 << "-я дробь: ";
			d[i] = InputUserFract();
		}
	}
	static void Output()
	{
		cout << "\n " << setw(23) << "Введено пять дробей:"; PrintDeque();
	}
	static void Sum()
	{
		TFract summ = accumulate(d.begin(), d.end(), TFract(), plus<TFract>());
		cout << "\n\n " << setw(23) << "Сумма всех дробей:" << "\t" << summ.Display() << endl;
	}
	static void SortUp()
	{
		sort(d.begin(), d.end(), less<TFract>());
		cout << "\n " << setw(23) << "Сорт. по возрастанию:"; PrintDeque();
	}
	static void SortDown()
	{
		sort(d.begin(), d.end(), greater<TFract>());
		cout << "\n " << setw(23) << "Сорт. по убыванию:"; PrintDeque();
	}
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	InOutDo::Input();
	InOutDo::Output();
	InOutDo::SortUp();
	InOutDo::SortDown();
	InOutDo::Sum();

	cout << endl;
	system("pause");

	return 0;
}