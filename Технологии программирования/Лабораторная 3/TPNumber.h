#ifndef TPNumber_H
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <windows.h>
#include <ctime>
#include <algorithm>

using namespace std;

string GetSymbol(int n) {
	switch (n) {
	case 10: return "A"; break;
	case 11: return "B"; break;
	case 12: return "C"; break;
	case 13: return "D"; break;
	case 14: return "E"; break;
	case 15: return "F"; break;
	}
	return to_string(n);
}

class TPNumber {
private:
	double n;
	int p, c;
public:
	class TPNumErr {
	public:
		const char* msg;
		TPNumErr(const char* msg) { this->msg = msg; }
	};
	TPNumber(double a = 1, int b = 10, int c = 0) {

		if (b < 2 || b>16 || c < 0 || c>20) throw TPNumErr("Ошибка инициализации");

		n = a;	p = b;	this->c = c;
	}
	TPNumber(string a, string b, string c) {

		*this = TPNumber(atof(a.c_str()), atoi(b.c_str()), atoi(c.c_str()));
	}
	string GetNumStr() {
		ostringstream os;
		int a = abs(int(n));
		double b = abs(n) - a;
		do {
			os << GetSymbol(a % p);
			if ((a /= p) < p) {
				os << GetSymbol(a);
				break;
			}
		} while (true);
		string rez = os.str();
		reverse(rez.begin(), rez.end());

		if (rez[0] == '0') rez.erase(0, 1);

		if (b && c) {				// если есть дробная часть и точность > 0
			int count = 0;
			ostringstream os;
			b *= p;
			do {
				os << GetSymbol(int(b));
				b -= int(b);
				if (b - int(b) == 0 || ++count == c) {
					rez += '.' + os.str();
					break;
				}
				b *= p;
			} while (true);

			while (rez[rez.length() - 1] == '0')
				rez.erase(rez.length() - 1, rez.length());

			if (rez[rez.length() - 1] == '.')
				rez.erase(rez.length() - 1, rez.length());
		}

		if (n < 0) rez = '-' + rez;

		return rez;
	}

	TPNumber GetCopy() { return *this; }

	TPNumber operator +(const TPNumber& op2) const {

		if (p != op2.p || c != op2.c)
			throw TPNumErr("Точность и основание операндов должны совпадать!");

		return TPNumber(n + op2.n, p, c);
	}

	TPNumber operator *(const TPNumber& op2) const {

		if (p != op2.p || c != op2.c)
			throw TPNumErr("Точность и основание операндов должны совпадать!");

		return TPNumber(n * op2.n, p, c);
	}

	TPNumber operator -(const TPNumber& op2) const {

		if (p != op2.p || c != op2.c)
			throw TPNumErr("Точность и основание операндов должны совпадать!");

		return TPNumber(n - op2.n, p, c);
	}

	TPNumber operator /(const TPNumber& op2) const {

		if (op2.n == 0) throw TPNumErr("Делитель равен нулю");

		if (p != op2.p || c != op2.c)
			throw TPNumErr("Точность и основание операндов должны совпадать");

		return TPNumber(n / op2.n, p, c);
	}

	TPNumber GetReverse() {

		if (n == 0) throw TPNumErr("Делитель равен нулю");

		return TPNumber(1 / n, p, c);
	}

	TPNumber GetSqr() { return TPNumber(n * n, p, c); }

	double GetNum() { return n; }

	int GetBase() { return p; }

	string GetBaseStr() { return to_string(p); }

	int GetPrecision() { return c; }

	string GetPrecisionStr() { return to_string(c); }

	void SetBase(int p) {

		if (p < 2 || p > 16) throw TPNumErr("Основание должно быть от 2 до 16");

		this->p = p;
	}

	void SetBaseStr(string newp) {

		int base = atoi(newp.c_str());

		if (base < 2 || base > 16) throw TPNumErr("Основание должно быть от 2 до 16");

		p = base;
	}

	void SetPrecision(int c) {

		if (c < 0 || c > 20) throw TPNumErr("Точность должна быть от 0 до 20");

		this->c = c;
	}

	void SetPrecisionStr(string newc) {

		int prec = atoi(newc.c_str());

		if (prec < 0 || prec > 20) throw TPNumErr("Точность должна быть от 0 до 20");

		p = prec;
	}
};
#endif
