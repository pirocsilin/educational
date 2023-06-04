#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <fstream>

using namespace std;

struct Edge {
	int u, v, w;
	Edge(int x, int y, int z) : u(x), v(y), w(z) {}
};

const int V0 = 1, n = 10;
double inf = numeric_limits<double>::infinity();
const char* fname = "graph2.txt";
vector<double> MinPrice(n, inf);
vector<int>Path(n, -1);
vector<Edge> E;
int m[n][n];

string GetPath(int e) {
	string rez;
	for (int val = e; val != -1; val = Path[val]) {
		rez = to_string(val) + " - " + rez;
	}
	return "{ " + rez.erase(rez.size() - 2) + "}";
}

void GetMinPrice() {
	MinPrice[V0] = 0;
	bool stable = false;
	while (!stable) {
		stable = true;
		for (int j = 0; j < E.size(); j++) {
			if (MinPrice[E[j].v] + E[j].w < MinPrice[E[j].u]) {
				MinPrice[E[j].u] = MinPrice[E[j].v] + E[j].w;
				Path[E[j].u] = E[j].v;
				stable = false;
			}
		}
	}
}

void SetMatrix() {
	ifstream in(fname);
	if (!in.is_open()) {
		cout << "\n Файл " << fname << " не может быть открыт\n" << endl;
		system("pause"); 
		exit(-1);
	}
	string str;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			in >> m[i][j];
		getline(in, str);
	}
	in.close();
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	SetMatrix();

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (m[i][j] != 0)
				E.push_back(Edge(i, j, m[i][j]));

	GetMinPrice();

	cout << "\n Минииальные пути:" << endl;
	for (int i = 0; i < n; i++) {
		if (i == V0) continue;
		cout << "\n от " << V0 << " до " << i << ": " << MinPrice[i] << '\t' << GetPath(i);
	}

	cout << endl << endl;
	system("pause");

	return 0;
}