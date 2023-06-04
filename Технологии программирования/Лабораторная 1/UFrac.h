#ifndef UFract_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <windows.h>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <functional>
#include <numeric>

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
			throw ErrFract("ƒелитель равен нулю");

		int sign = op2.num < 0 ? -1 : 1;

		TFract b = TFract(op2.denom * sign, op2.num * sign);

		return *this * b;
	}
	bool operator == (const TFract& op2) const {

		return num * op2.denom + denom * op2.num - denom * op2.denom == 0;
	}
	bool operator > (const TFract& op2) const {

		return (*this - op2).num > 0;
	}
};
#endif