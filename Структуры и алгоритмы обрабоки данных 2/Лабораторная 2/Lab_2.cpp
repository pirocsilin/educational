#include <iostream>
#include <list>
#include <conio.h>
#include <windows.h>
#include <cmath>
using namespace std;
// установка кодировки для консольного вывода
#define SET_CP SetConsoleCP(1251); SetConsoleOutputCP(1251);

struct Vertex				// структура для вершины ИСДП
{
	int V;
	struct Vertex* ptr_Left;
	struct Vertex* ptr_Right;
} *Root;

struct Vertex_AVL			// структура для вершины АВЛ
{
	int V;
	int Balance;
	struct Vertex_AVL* ptr_Left;
	struct Vertex_AVL* ptr_Right;
} *Root_AVL;

list<int> Array;
list<int>::iterator it;

void Comparison();
void Create_AVL();

int main()
{
	SET_CP; char ch;
	system("mode con cols=83 lines=25");

	while (1)				// меню пользователя
	{
		system("cls");
		printf("\n%55s", ">> ПРОГРАММНОЕ ПОСТРОЕНИЕ АВЛ <<\n");
		cout << "\n 1 - Сравнение характеристик ИСДП и АВЛ";
		cout << "\n 2 - Построить АВЛ";
		cout << "\n 3 - Завершить\n ";
		ch = _getch();

		switch (ch)	
		{
		case '1': Comparison(); break;
		case '2': Create_AVL(); break;
		case '3': return 0;
		default: cout << "error";
		}
	}
}

template<typename T> int size_wood(T *p)			// размер дерева
{
	if (p == NULL)
		return 0;
	else
		return 1 + size_wood(p->ptr_Left) + size_wood(p->ptr_Right);
}

template<typename T> int height_wood(T *p)		// высота дерева
{
	if (p == NULL)
		return 0;
	else
		return 1 + max(height_wood(p->ptr_Left), height_wood(p->ptr_Right));
}

template<typename T> int average(T *p, int L)	// средняя высота
{
	if (p == NULL)
		return 0;
	else
		return L + average(p->ptr_Left, L + 1) + average(p->ptr_Right, L + 1);
}

template<typename T> int control_summ(T *p)		// контрольная сумма
{
	if (p == NULL)
		return 0;
	else
		return p->V + control_summ(p->ptr_Left) + control_summ(p->ptr_Right);
}

template<typename T> void print_wood(T *p)		// обход дерева
{
	if (p != NULL)
	{
		print_wood(p->ptr_Left);
		cout << p->V << " ";
		print_wood(p->ptr_Right);
	}
}

template<typename T> bool check_poisk(T *p)		// проверка на дерево поиска
{
	if (p != NULL && ((p->ptr_Left != NULL && (p->V > p->ptr_Left->V
		|| check_poisk(p->ptr_Left)))
		|| (p->ptr_Right != NULL && (p->V < p->ptr_Right->V
			|| check_poisk(p->ptr_Right)))))
		return true;

	return false;
}

template<typename T> void find_elem(T *p, int n)	// поиск элемента
{
	while (p != NULL)
	{
		if (p->V > n) p = p->ptr_Left;
		else if (p->V < n) p = p->ptr_Right;
		else break;
	}
	if (p == NULL) printf("\t  [%d] Нет элемента", n);
	else printf("\t  [%d] Элемент найден", n);
}

struct Vertex* ESDP(int L, int R)				// создание ИСДП
{
	if (L > R) return NULL;

	int m = ceil((L + (double)R) / 2);
	auto Elem = Array.begin();
	auto nx = next(Elem, m - 1);

	struct Vertex* p = new Vertex;
	p->V = *nx;
	p->ptr_Left = ESDP(L, m - 1);
	p->ptr_Right = ESDP(m + 1, R);

	return p;
}
/*Функция выполняет однократный поворот между узлом и его левым ребенком, 
 корректирует высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* LL(struct Vertex_AVL *p)		// LL поворот
{
	struct Vertex_AVL* q = p->ptr_Left;
	p->ptr_Left = q->ptr_Right;
	q->ptr_Right = p;

	p->Balance = height_wood(p);		// корректировка высот
	q->Balance = height_wood(q);

	return q;
}
/*Функция выполняет однократный поворот между узлом и его правым ребенком,
 корректирует высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* RR(struct Vertex_AVL* p)		// RR поворот
{
	struct Vertex_AVL* q = p->ptr_Right;
	p->ptr_Right = q->ptr_Left;
	q->ptr_Left = p;

	p->Balance = height_wood(p);		// корректировка высот
	q->Balance = height_wood(q);

	return q;
}
/*Функция выполняет двойной LR поворот, корректирует 
высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* LR(struct Vertex_AVL* p)		// LR поворот
{
	p->ptr_Left = RR(p->ptr_Left);
	return LL(p);
}
/*Функция выполняет двойной RL поворот, корректирует
высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* RL(struct Vertex_AVL* p)		// RL поворот
{
	p->ptr_Right = LL(p->ptr_Right);
	return RR(p);
}
/*Функция добавляет узел в АВЛ дерево, возвращает указатель на корень*/
struct Vertex_AVL* AVL(int x, struct Vertex_AVL* p)	// создание АВЛ
{
	if (p == NULL)
	{
		p = new Vertex_AVL();
		p->V = x;
		p->ptr_Left = NULL;
		p->ptr_Right = NULL;
		p->Balance = 0;
	}
	else if (x < p->V)
	{
		p->ptr_Left = AVL(x, p->ptr_Left);
		if (height_wood(p->ptr_Left) - height_wood(p->ptr_Right) == 2)
			if (x < p->ptr_Left->V)
				p = LL(p);
			else
				p = LR(p);
	}
	else if (x > p->V)
	{
		p->ptr_Right = AVL(x, p->ptr_Right);
		if (height_wood(p->ptr_Right) - height_wood(p->ptr_Left) == 2)
			if (x > p->ptr_Right->V)
				p = RR(p);
			else
				p = RL(p);
	}
	return p;
}

void create_array(int n)						// создание массива чисел
{
	while (Array.size() != n)
	{
		int random_n = rand();
		it = find(Array.begin(), Array.end(), random_n);
		if (it != Array.end()) continue;
		Array.push_back(random_n);
	}
}

template<typename T> void Delete_Wood(T *p)	// удаление дерева
{
	if (p != NULL)
	{
		Delete_Wood(p->ptr_Left);
		Delete_Wood(p->ptr_Right);
		delete p;
	}
}

void Create_AVL()							// характеристики АВЛ
{
	char digit_to_string[30];
	int n, exit = 1;

	do
	{
		system("cls");
		printf("\n%55s", ">> ПРОГРАММНОЕ ПОСТРОЕНИЕ АВЛ <<\n");
		cout << "\n Количество вершин дерева N (<= 32767): ";
		cin >> digit_to_string;
		n = atoi(digit_to_string);
	} while (n > 32767);

	create_array(n);

	Root_AVL = NULL;
	for (int elem : Array)
		Root_AVL = AVL(elem, Root_AVL);

	cout << "\n Исходный массив:\t [ ";
	for (it = Array.begin(); it != Array.end(); it++) cout << *it << " ";
	cout << "]" << endl;

	cout << "\n    Оход дерева:\t [ ";
	print_wood(Root_AVL);
	cout << "]" << endl;

	cout << "\n Характеристики";
	cout << "\n\t  Размер дерева:\t" << size_wood(Root_AVL);
	cout << "\n\t  Высота дерева:\t" << height_wood(Root_AVL);
	cout << "\n\t Средняя высота:\t";
	if (size_wood(Root_AVL) == 0) cout << 0;
	else cout << average(Root_AVL, 1) / size_wood(Root_AVL);
	cout << "\n\t  Контр-я сумма:\t" << control_summ(Root_AVL);
	cout << "\n\tДерево поиска ?:\t";
	if (check_poisk(Root_AVL) == 0) cout << "False";
	else cout << "True";

	do
	{
		cout << "\n\n Элемент для поиска или N - Выход ";
		cin >> digit_to_string;
		for (char c : digit_to_string) if (c == 'N' || c == 'n') exit = 0;
		n = atoi(digit_to_string);
		find_elem(Root_AVL, n);
	} while (exit);

	Array.clear();
	Delete_Wood(Root_AVL);
	
}

void Comparison()		// сравнение характеристик АВЛ с ИСДП
{
	char ch;
	do
	{
		system("cls");
		printf("\n%65s", ">> СРАВНЕНИЕ ХАРАКТЕРИСТИК ДЕРЕВЬЕВ <<\n\n");
		printf("\n%32s%35s\n", "АВЛ", "ИСДП");
		cout << "\n Размер\t| Контр\t   Высота   Высота   Теор. оценки | Контр     Высота     Высота  |";
		cout << "\n дерева\t| сумма\t   фактич.  средняя  сред. высоты | сумма   факт./теор.  средняя |";
		cout << "\n\n";

		for (int i = 1; i <= 5; i++)
		{
			create_array(i * 100);

			Root_AVL = NULL;
			for (int elem : Array) Root_AVL = AVL(elem, Root_AVL);	

			printf("%5d", size_wood(Root_AVL));
			printf("%12d", control_summ(Root_AVL));
			printf("%7d", height_wood(Root_AVL));
			printf("%9d", average(Root_AVL, 1) / size_wood(Root_AVL));
			printf("%9d - %d", (int)ceil(log(i * 100+1) / log(2)), 
							   (int)ceil((1.44*log(i * 100 + 2) / log(2))-0.328));

			Array.sort();
			Root = ESDP(1, Array.size());

			printf("%11d", control_summ(Root));
			printf("%6d / ", height_wood(Root));
			printf("%d", (int)ceil(log(size_wood(Root) + 1) / log(2)));
			printf("%9d\n\n", average(Root, 1) / size_wood(Root));

			Array.clear();
			Delete_Wood(Root);
			Delete_Wood(Root_AVL);
		}
		cout << "\n\n ENTER - повторить, ESC - выход ";

	} while (_getch() != 27);
}
