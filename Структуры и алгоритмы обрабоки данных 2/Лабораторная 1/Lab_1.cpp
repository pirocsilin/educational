#include <iostream>
#include <list>
#include <conio.h>
#include <windows.h>
#include <cmath>
#include<string>
using namespace std;
// установка кодировки для консольного вывода
#define SET_CP SetConsoleCP(1251); SetConsoleOutputCP(1251);
// очистка буфера от лишних символов при сканировании консоли
#define cls while(getchar() != '\n');

struct Vertex					// структура для вершины дерева
{
	int V;
	struct Vertex* ptr_Left;
	struct Vertex* ptr_Right;

} *Root, *q;

list<int> Array;
list<int>::iterator it;

void Comparison();
void Create_ESDP_SDP(int);

int main()
{
	SET_CP;
	system("mode con cols=83 lines=25");
	char ch;	
	while (1)
	{
		system("cls");
		printf("\n%55s", ">> ПРОГРАММНОЕ ПОСТРОЕНИЕ ИСДП и СДП <<\n");
		cout<<"\n 1 - Сравнение характеристик ИСДП и СДП";
		cout << "\n 2 - Построить ИСДП";
		cout << "\n 3 - Построить СДП";
		cout << "\n 4 - Завершить\n ";
		ch = _getch();

		switch (ch)						// меню пользователя
		{
		case '1': Comparison(); break;
		case '2': Create_ESDP_SDP(0); break;
		case '3': Create_ESDP_SDP(1); break;
		case '4': return 0;
		default: cout<<"error";
		}
	}
}

int size_wood(struct Vertex* p)		// размер дерева
{
	if (p == NULL) 
		return 0;
	else
		return 1 + size_wood(p->ptr_Left) + size_wood(p->ptr_Right);
}

int height_wood(struct Vertex* p)	// высота дерева
{
	if (p == NULL)
		return 0;
	else
		return 1 + max(height_wood(p->ptr_Left), height_wood(p->ptr_Right));
}

int average(struct Vertex *p, int L)	// средняя высота
{
	if (p == NULL)
		return 0;
	else
		return L + average(p->ptr_Left, L + 1) + average(p->ptr_Right, L + 1);
}

int control_summ(struct Vertex* p)	// контрольная сумма
{
	if (p == NULL)
		return 0;
	else
		return p->V + control_summ(p->ptr_Left) + control_summ(p->ptr_Right);
}

void print_wood(struct Vertex *p)	// обход дерева
{
	if (p != NULL)
	{
		print_wood(p->ptr_Left);
		cout << p->V << " ";
		print_wood(p->ptr_Right);
	}
}

bool check_poisk(struct Vertex* p)	// проверка на дерево поиска
{
	if (p != NULL && ((p->ptr_Left != NULL && (p->V > p->ptr_Left->V
		|| check_poisk(p->ptr_Left)))
			|| (p->ptr_Right != NULL && (p->V < p->ptr_Right->V
				|| check_poisk(p->ptr_Right)))))
		return true;

	return false;
}

void find_elem(int n)				// поиск элемента
{
	struct Vertex* p = Root;
	while (p != NULL)
	{
		if (p->V > n) p = p->ptr_Left;
		else if (p->V < n) p = p->ptr_Right;
		else break;
	}
	if(p==NULL) printf("\t  [%d] Нет элемента", n);
	else printf("\t  [%d] Элемент найден", n);
}

struct Vertex * ESDP(int L, int R)	// создание ИСДП
{
	if (L > R) return NULL;

	int m = ceil((L + (double)R) / 2);
	auto Elem = Array.begin();
	auto nx = next(Elem, m-1);

	struct Vertex *p = new Vertex;
	p->V = *nx;
	p->ptr_Left = ESDP(L, m - 1);
	p->ptr_Right = ESDP(m + 1, R);

	return p;
}

struct Vertex * SDP(int x, struct Vertex *p)		// создание СДП
{
	if (p == NULL)
	{
		p = new Vertex();
		p->V = x;
		p->ptr_Left = NULL;
		p->ptr_Right = NULL;
		if (Root == NULL) Root = p;
	}
	else if (x < p->V) p->ptr_Left = SDP(x, p->ptr_Left);
	else p->ptr_Right = SDP(x, p->ptr_Right);

	return(p);
}

void create_array(int n)				// создание массива случайных чисел
{
	while (Array.size() != n)
	{
		int random_n = rand();
		it = find(Array.begin(), Array.end(), random_n);
		if (it != Array.end()) continue;
		Array.push_back(random_n);
	}
}

void Delete_Wood(struct Vertex *p)	// удаление дерева
{
	if (p != NULL)
	{
		Delete_Wood(p->ptr_Left);
		Delete_Wood(p->ptr_Right);
		delete p;
	}
}

void Create_ESDP_SDP(int Choice = 0)	// пункты меню 2,3 - Построить ИСДП, СДП
{
	char digit_to_string[10];
	string name;
	int n, exit = 1;
	if (Choice == 1) name = "СДП"; else name = "ИСДП";

	do
	{
		system("cls");
		printf("\n%55s %s <<\n", ">> ПРОГРАММНОЕ ПОСТРОЕНИЕ", name.c_str());
		cout <<"\n Количество вершин дерева N (<= 32767): ";
		cin>>digit_to_string; cls;
		n = atoi(digit_to_string);
	} while (n > 32767);

	create_array(n);

	if (Choice == 1)
	{
		Root = NULL;
		for (int elem : Array) SDP(elem, Root);
	}
	else
	{
		Array.sort();
		Root = ESDP(1, Array.size());
	}

	cout << "\n Исходный массив:\t [ ";
	for (it = Array.begin(); it != Array.end(); it++) cout << *it << " ";
	cout << "]" << endl;

	cout << "\n    Оход дерева:\t [ "; 
	print_wood(Root); 
	cout << "]" << endl;

	cout << "\n Характеристики";
	cout << "\n\t  Размер дерева:\t" << size_wood(Root);
	cout << "\n\t  Высота дерева:\t" << height_wood(Root);
	cout << "\n\t Средняя высота:\t"; 
	if (size_wood(Root) == 0) cout << 0; 
		else cout << average(Root, 1) / size_wood(Root);
	cout << "\n\t  Контр-я сумма:\t" << control_summ(Root);
	cout << "\n\tДерево поиска ?:\t";
	if (check_poisk(Root) == 0) cout << "False"; 
		else cout << "True";
	do
	{
		cout << "\n\n Элемент для поиска или N - Выход ";
		cin >> digit_to_string; cls;
		for (char c : digit_to_string) if (c == 'N' || c == 'n') exit = 0;
		n = atoi(digit_to_string);
		find_elem(n);
	} while (exit);

	Array.clear();
	Delete_Wood(Root);
}

void Comparison()
{
	do
	{
		system("cls");
		printf("\n%65s", ">> СРАВНЕНИЕ ХАРАКТЕРИСТИК ДЕРЕВЬЕВ <<\n\n");
		printf("\n%32s%35s\n", "СДП", "ИСДП");
		cout << "\n Размер\t| Контр\t   Высота   Высота   Теор. оценки | Контр     Высота     Высота  |";
		cout << "\n дерева\t| сумма\t   фактич.  средняя  сред. высоты | сумма   факт./теор.  средняя |";
		cout << "\n\n";

		for (int i = 1; i <= 5; i++)
		{
			create_array(i*100);

			Root = NULL;
			for (int elem : Array) SDP(elem, Root);

			printf("%5d", size_wood(Root));
			printf("%11d", control_summ(Root));
			printf("%7d", height_wood(Root));
			printf("%9d", average(Root, 1) / size_wood(Root));
			printf("%9d - %d", (int)ceil(log(i*100) / log(2)),i*100);

			Delete_Wood(Root);

			Array.sort();
			Root = ESDP(1, Array.size());

			printf("%11d", control_summ(Root));
			printf("%6d / ", height_wood(Root));
			printf("%d", (int)ceil(log(size_wood(Root) + 1) / log(2)));
			printf("%9d\n\n", average(Root, 1) / size_wood(Root));

			Array.clear();
			Delete_Wood(Root);
		}
		cout << "\n\n ENTER - повторить, ESC - выход ";

	} while (_getch() != 27);
}