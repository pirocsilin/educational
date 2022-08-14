#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <fstream>
#include <vector>
#include <tuple>
#include <set>

using namespace std;

struct Point { 
	int x, y, z;

	Point(int a=0, int b=0, int c=0): x(a), y(b), z(c) {}

	bool operator<(const Point& two) const noexcept
	{
		return tie(x, y) < tie(two.x, two.y);
	}
};
set<Point> V;

const int n = 8,
		  N = 13 * 47,
		  d = 7;

char Choice{};

vector<int> path(n),
			nodes(8, 0),
			Peaks{ 6, 3, 4, 2, 0, 1, 7, 5};			// Исходная перестановка вершин

int G[n][n] = { {0, 0, 1, 0, 0, 1, 1, 1},			// Исходный граф
				{0, 0, 1, 1, 0, 0, 0, 1},
				{1, 1, 0, 0, 1, 1, 1, 0},
				{0, 1, 0, 0, 0, 1, 0, 0},
				{0, 0, 1, 0, 0, 0, 1, 1},
				{1, 0, 1, 1, 0, 0, 0, 0},
				{1, 0, 1, 0, 1, 0, 0, 0},
				{1, 1, 0, 0, 1, 0, 0, 0} };

int  H[n][n]{};										// Изоморфный граф
int _H[n][n]{};										// Кодированная иатрица
int  F[n][n]{};										// Шифрованная матрица _H
int  EncodPath[n][n]{};								// Здесь будем хранить шифрованный путь
Point EncodEdges[n]{};								// Список кодированных ребер

bool CheckGraph(int v, int G[][n]) {

	path.push_back(v);
	if (path.size() == n)
		if (G[path[0]][path[n - 1]])
			return true;
		else {
			path.pop_back();
			false;
		}

	nodes[v] = 1;						// Отмечаем вершину как посещенную
	for (int i = 0; i < n; i++)			// Перебираем вершины для поиска дальнейшего пути
		if (G[v][i] && !nodes[i])		// Если есть ребро и вершину еще не посещали
			if (CheckGraph(i, G))		// то пытаемся найти продолжение пути от неё
				return true;

	nodes[v] = 0;
	//if(!path.empty()) 
	path.pop_back();
	return false;
}

int FastPower(int a, int x, int p) {

	int LenBin = log2(x) + 1, y = 1, s = a;

	while (LenBin--) {
		if (x & 1) y = y * s % p;
		s = s * s % p;
		x >>= 1;
	}
	return y;
}

void GetPath(int Graph[][n]) {

	path.clear();	

	if (!CheckGraph(0, Graph)) {
		cout << "\n\tГраф не имеет Гамильтонов цикл!";
		system("pause");
		exit(-1);
	}
	cout << "\n\tГамильтонов цикл:";
	for (int val : path) printf("%3d", val);

	cout << endl;		

	//cout << "\tCoords the path\n";
	srand(77);
	for (int i = n-1; i < 2*n-1; i++) {						// Координаты точек Гамильтонова пути
		V.insert(Point(path[i % 8], path[(i + 1) % 8]));
		//cout << path[i % 8] << ", " << path[(i + 1) % 8] << endl;
	}
	//cout << endl;
	//for(auto v : V) cout << v.x << ", " << v.y << endl;

}

void PrintGraph(int G[][n]) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%4d", G[i][j]);
		printf("\n");
	}
}

void GetIsomorphicGraph(int A[][n], int B[][n]) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			A[Peaks[i]][Peaks[j]] = B[i][j];					// Получаем изоморфный граф
			if (V.find(Point(i, j)) != V.end())				// и его Гамильтонов цикл
				EncodPath[Peaks[i]][Peaks[j]] = B[i][j];		}
}

void GetEncodedGraph(int G[][n]) {
	srand(77);
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++) {
			int n = (rand() % 9 + 1) * 10;
			_H[i][j] = H[i][j] + n;							// Кодируем изоморфный граф
			if (EncodPath[i][j]) {							// и его Гамильтонов цикл
				EncodPath[i][j] += n;						// + формируем список для Боба
				EncodEdges[i] = Point(i, j, EncodPath[i][j]);
			}
		}
}
													
void GetEncryptedGraph(int A[][n], int B[][n]) {
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			A[i][j] = FastPower(B[i][j], d, N);
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=90 lines=170");

	cout << "\n\tОсновной граф G:\n\n"; 
	PrintGraph(G);
	GetPath(G);

	cout << "\n\tАлиса выбрала случайную нумерацию вершин: ";

	for (int i = 0; i < n; i++) {
		Peaks[i] = (Peaks[i] + 2) % 8;			// Вариант 1, но так как счет вершин от 0, то еще +1
		cout << Peaks[i] << " ";
	}

	cout << "\n\tИ построила изоморфный граф H:\n\n";
	
	GetIsomorphicGraph(H, G);
	PrintGraph(H);

	cout << "\n\tЗакодированная матрица H':\n\n";

	GetEncodedGraph(H);
	PrintGraph(_H);

	cout << "\n\tЗакодированный путь:\n\n";
	PrintGraph(EncodPath);								// выводим закодированный путь!

	cout << "\n\tЗашифрованная матрица F:\n\n";

	GetEncryptedGraph(F, _H);
	PrintGraph(F);

	cout << "\n\tАлиса отправила Бобу матрицу F. Боб задает вопрос Алисе:\n";

	do {
		Choice = 0;
		cout << "\n\t\t1. Действительно ли граф Н изоморфен G?";
		cout << "\n\t\t2. Каков гамильтонов цикл для графа Н?	";
		while (Choice != '1' && Choice != '2' && Choice != 27) { Choice = _getch(); };
		switch (Choice) {
		case '1': {
			cout << Choice << "\n\n\tАлиса выслала Бобу матрицу H' и перестановки";

			cout << "\n\n\t\tПроверка 1: ";					// проверка идентичности матриц
			int TempMatrix[n][n]{}, Check{};
			GetEncryptedGraph(TempMatrix, _H);
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					if (F[i][j] ^ TempMatrix[i][j]) {
						cout << "матрица H' не соответствует F";
						system("pause");
						exit(-1);
					}
			cout << "матрица H' соответствует F";

			cout << "\n\t\tПроверка 2: ";					// проверка изоморфности графов
			GetIsomorphicGraph(TempMatrix, G);
			for (int i = 0; i < n; i++)
				for (int j = 0; j < n; j++)
					if (_H[i][j] & 1 ^ TempMatrix[i][j]) {
						cout << "граф Н не изоморфен G";
						system("pause");
						exit(-1);
					}
			cout << "граф Н изоморфен G";
		}
		break;

		case '2': {
			cout << Choice << "\n\n\tАлиса выслала Бобу список закодированных ребер";

			cout << "\n\n\t\tПроверка 1: ";					// проверка соответствия ребер матрице F
			
			for (auto v : EncodEdges)
				if(FastPower(v.z, d, N != F[v.x][v.y])) {
					cout << "ребра не соответствуют матрице F";
						system("pause");
						exit(-1);
				}
			cout << "ребра соответствуют матрице F";

			cout << "\n\t\tПроверка 2: ";					// проверка валидности пути
			for (auto A : EncodEdges) {
				bool TryPath = false;
				for (auto B : EncodEdges)
					if ((A.x != A.y) && (B.x != B.y) && (A.x != B.y) && (A.y == B.x)) {
						TryPath = true;
						break;
					}
				if (!TryPath) {
					cout << "присланный путь не является Гамильтоновым циклом";
					system("pause");
					exit(-1);
				}
			}
			cout << "путь проходит через все вершины графа по одному разу";
		}
		break;

		}
		cout << endl;

	} while (Choice != 27);

	cout << endl;

	return 0;
}