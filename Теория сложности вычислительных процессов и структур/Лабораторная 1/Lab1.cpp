#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>
#include <windows.h>
#include <iomanip>
#include <fstream>

using namespace std;

struct Edge {
	int a, b, w;
	Edge(int x, int y, int z) : a(x), b(y), w(z) {}

	bool operator < (const Edge& op2) const {
		return w < op2.w;
	}
};

const char* fname = "graph1.txt";
const int n = 10;
int m[n][n];

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

	set<int> s[n];			// Компоненты связности
	vector<Edge> e, o;		// Все грани; Остов

	SetMatrix();

	for (int i = 0; i < n; i++) {
		s[i].insert(i + 1);
		for (int j = i + 1; j < n; j++) {
			if (m[i][j])
				e.push_back(Edge(i + 1, j + 1, m[i][j]));
		}
	}
	sort(e.begin(), e.end());

	for (int i = 0; i < e.size(); i++) {

		int Set1, Set2;
		for (int j = 0; j < n; j++) {

			if (s[j].count(e[i].a)) Set1 = j;

			if (s[j].count(e[i].b)) Set2 = j;
		}
		if (Set1 != Set2) {		// Если концы ребра в разных компонентах связности
			o.push_back(e[i]);
			s[Set1].insert(s[Set2].begin(), s[Set2].end());
			s[Set2].clear();
		}
	}
	int sum = 0;
	cout << "\n Порядок добавления ребер / вес:\n\n";
	for (int i = 0; i < o.size(); i++) {
		cout << setw(14) << i+1 <<") " << o[i].a 
			 << " - " << o[i].b << "\t/  " << o[i].w << endl;
		sum += o[i].w;
	}

	cout << "\n Вес остова: " << sum << endl;

	cout << endl;
	system("pause");
	return 0;
}