#ifndef Ucomplex_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <windows.h>
#include <ctime>
#include <algorithm>
#include <iomanip>
#define PI 3.14159265358979323846

using namespace std;

double& SetPrec(double n, int p) {
	n = round(n * p) / p;

	return n;
}

class TComplex {
private:
	double Re;
	double Im;
public:
	friend double& SetPrec(double, int p);
	class ErrComplex {
	public:
		const char* msg;
		ErrComplex(const char* e) : msg(e) {}
	};
	TComplex() : Re(0), Im(0) {}

	TComplex(double a, double b) {
		Re = SetPrec(a, 1000);
		Im = SetPrec(b, 1000);
	}

	TComplex(string str) {
		char ch;
		istringstream is(str);
		is >> Re >> Im >> ch;
		if (ch != 'i') throw ErrComplex("Ошибка инициализации");

		*this = TComplex(Re, Im);
	}
	string GetComStr() {
		string sign = Im < 0 ? "" : "+";
		ostringstream os;
		os << Re << sign << Im << "i";

		return os.str();
	}
	TComplex operator + (const TComplex& b) const {
		return TComplex(Re + b.Re, Im + b.Im);
	}
	TComplex operator - (const TComplex& b) const {
		return TComplex(Re + (b.Re * -1), Im + (b.Im * -1));
	}
	TComplex operator * (const TComplex& b) const {
		return TComplex(Re * b.Re + Im * b.Im * -1, Re * b.Im + Im * b.Re);
	}
	TComplex operator / (const TComplex& b) const {
		double A = (Re * b.Re + Im * b.Im) / (b.Re * b.Re + b.Im * b.Im);
		double B = (Im * b.Re - Re * b.Im) / (b.Re * b.Re + b.Im * b.Im);
		return TComplex(A, B);
	}
	TComplex operator - () const {
		return TComplex() - *this;
	}
	TComplex GetSqrC() {
		return TComplex(*this * *this);
	}
	TComplex GetReverseC() {
		return TComplex(Re / (Re * Re + Im * Im), -Im / (Re * Re + Im * Im));
	}
	double GetModule() {
		return sqrt(Re * Re + Im * Im);
	}
	double GetAngleRad() {
		double fi;
		if (Re > 0)
			fi = atan(Im / Re);
		else if (Re == 0 && Im > 0)
			fi = PI / 2;
		else if (Re < 0)
			fi = PI + atan(Im / Re);
		else if (Re == 0 && Im < 0)
			fi = -PI / 2;
		else
			fi = 0;

		return fi;
	}
	double GetAngleGrad() {
		return SetPrec((*this).GetAngleRad() * 180 / PI, 100);
	}
	TComplex GetPow(int n) {

		double fa = pow((*this).GetModule(), n);
		double fi = (*this).GetAngleRad();

		return TComplex(fa * cos(n * fi), fa * sin(n * fi));
	}
	TComplex Get_i_Root(int n, int i) {

		if (n < 2 || --i < 0 || i > n - 1) throw ErrComplex("Не верно указаны параметры");

		double pow_fi = pow((*this).GetModule(), 1. / n);
		double a = pow_fi * cos(((*this).GetAngleRad() + 2 * PI * i) / n);
		double b = pow_fi * sin(((*this).GetAngleRad() + 2 * PI * i) / n);

		return TComplex(a, b);
	}
	bool operator == (const TComplex& b) const {

		return Re == b.Re && Im == b.Im;
	}
	bool operator != (const TComplex& b) const {

		return !(*this == b);
	}

	double GetRe() { return Re; }

	double GetIm() { return Im; }

	string GetReStr() { return to_string(Re); }

	string GetImStr() { return to_string(Im); }

	TComplex GetCopyC() { return *this; }

};
#endif