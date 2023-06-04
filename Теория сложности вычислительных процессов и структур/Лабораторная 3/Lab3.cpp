#include <windows.h>
#include <iostream>
#include <vector>

using namespace std;

const int M = 43, n = 4;

struct Prod {
	int m, c;
	Prod(int a, int b) : m(a), c(b) {}
};

struct Rez {
	int m, p[n];
	Rez() : m(0), p{0,0,0,0} {}
};

Prod P[n] = { {Prod(7,28)},
			  {Prod(9,43)},
			  {Prod(12,51)},
			  {Prod(8,34)} };

vector<Rez> f(M + 1, Rez());

void Calculation() {
	for (int i = 0; i <= M; i++)
		for (int k = 0; k < n; k++)
			if (i - P[k].m >= 0 && f[i - P[k].m].m + P[k].c > f[i].m) {
				f[i] = f[i - P[k].m];
				f[i].m += P[k].c;
				f[i].p[k] ++;
			}
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=50 lines=50");

	Calculation();

	cout << " Масса" << "  Цена" << "  Набор(7,9,12,8)\n" << endl;
	for (int i = 0; i <= M; i++) {
		cout << ' ' << i << '\t' << f[i].m << "\t";
		for (int val : f[i].p)
			cout << val << "  ";
		cout << endl;
	}

	cout << endl;
	system("pause");
	return 0;
}