#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <windows.h>

using namespace std;

const int n = 12;
const char* fname = "matrices.txt";
int F[n + 1][n + 1]{};				// результаты min трудоемкостей произведения
string P[n]{};						// расстановка скобок

struct Matrix {			// структура для хранения размерности матрицы
	int m, n;
	Matrix(int a=0, int b=0) : m(a), n(b) {}
} M[n];

struct Rezult {			// хранение матриц на которых получены минимальные трудоемкости
	int L[2], R[2];
	Rezult(int a = 0, int b = 0, int c = 0, int d = 0) {
		L[0] = a; L[1] = b;
		R[0] = c; R[1] = d;
	}
}R[n + 1][n + 1];

void GetMatrices() {		// считываем из файла размерности матриц
	ifstream in(fname);
	if (!in.is_open()) {
		cout << "\n Файл " << fname << " не может быть открыт\n" << endl;
		system("pause");
		exit(-1);
	}
	string str;
	char ch;
	int a, b;
	for (int i = 0; i < n; i++) {
		in >> a >> ch >> b;
		M[i] = Matrix(a, b);
		getline(in, str);
	}
	in.close();
}

int Mul(int a, int b, int c) {return M[a - 1].m * M[b - 1].n * M[c - 1].n;}

void InterRezult(int k, int t, int j) {
	printf("\n f[%d][%d] = f[%d][%d] + f[%d][%d] + %d*%d*%d = %d", 
		k, k + t, k, j, j + 1, k + t, M[k - 1].m, M[j - 1].n, M[k + t - 1].n, F[k][k + t]);
}

pair<int, int>  Calc() {
	int a(0), b(0), c(0);
	for (int t = 1; t <= n - 1; t++)
		for (int k = 1; k <= n - t; k++) {
			for (int j = k; j <= k + t - 1; j++) {
				if (F[k][k + t] > F[k][j] + F[j + 1][k + t] + Mul(k, j, k + t)) {
					F[k][k + t] = F[k][j] + F[j + 1][k + t] + Mul(k, j, k + t);
					R[k][k + t].L[0] = k; R[k][k + t].L[1] = j;
					R[k][k + t].R[0] = j + 1; R[k][k + t].R[1] = k + t;
					a = k; b = t; c = j;
				}
			}
			InterRezult(a, b, c);
		}
	return pair<int, int>(a, a + b);
}

void Rez(int a, int b) {
	if (R[a][b].L[0] - R[a][b].L[1] != 0) {
		P[R[a][b].L[0] - 1] += '(';
		P[R[a][b].L[1] - 1] += ')';
		Rez(R[a][b].L[0], R[a][b].L[1]);
	}
	if (R[a][b].R[0] - R[a][b].R[1] != 0) {
		P[R[a][b].R[0] - 1] += '(';
		P[R[a][b].R[1] - 1] += ')';
		Rez(R[a][b].R[0], R[a][b].R[1]);
	}
}

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=74 lines=76");

	GetMatrices();

	for (int i = 0; i < n + 1; i++) {
		for (int j = i; j < n + 1; j++) {
			if (i != j) F[i][j] = INT_MAX;
		}
	}

	cout << "\n Промежуточные результаты min трудоемкостей:\n";
	auto AB = Calc();

	Rez(AB.first, AB.second);

	ostringstream os;
	for (int i = 0; i < n; i++) {
		if (P[i].find('(') < n)
			os << P[i] << 'M' << i + 1 << '*';
		else if (P[i].find(')') < n)
			os << 'M' << i + 1 << P[i] << '*';
		else
			os << 'M' << i + 1 << '*';
	}
	string rezult = os.str();

	cout << "\n\n " << rezult.erase(rezult.size() - 1) << endl;
	cout << "\n Трудоёмкость = " << F[AB.first][AB.second] << endl;

	cout << endl;
	return 0;
}