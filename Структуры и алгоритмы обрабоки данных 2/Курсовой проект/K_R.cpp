#include <string>
#include <conio.h>
#include <iostream>
#include <windows.h>
#include <iomanip>
#include <map>
#include <set>
#include <list>
#pragma warning(disable : 4996)

#define SET_CP_ON  setlocale(LC_ALL, "Rus");
#define SET_CP_OFF setlocale(0, "C");

using namespace std;

int DB_Load();
void Load_Case(int Key = 1);

const char* fileName = "BASE1.dat";
int n = 1;

struct DB_Read				// структура для считывания из БД
{
	char Author[12];
	char Title[32];
	char Publisher[16];
	short Year;
	short Pages;
} New_Value;

struct AVL_Node				// структура для данных AVL
{
	char Author[13];
	char Title[33];
	char Publisher[17];
	short Year;
	short Pages;
} New_ValueNew;

struct Vertex_AVL			// структура для вершины AVL
{
	struct AVL_Node Data;
	int Balance;
	struct Vertex_AVL* Next;
	struct Vertex_AVL* ptr_Left;
	struct Vertex_AVL* ptr_Right;
} *Root_AVL = NULL;

int main()
{
	DB_Load();				// загружаем базу данных
	char ch;
	while (1)				// меню пользователя
	{
		SET_CP_ON;
		system("mode con cols=80 lines=25");
		system("cls");
		cout << "\n" << setw(50) << ">> КУРСОВАЯ РАБОТА <<\n";
		cout << "\n 1 - Список издательств и количество книг";
		cout << "\n 2 - Найти книги автора в издательстве";
		cout << "\n 3 - Завершить\n ";
		ch = _getch();

		switch (ch)
		{
		case '1': Load_Case(1);
				  break;
		case '2': Load_Case(2);
				  break;
		case '3': return 0;
		default: cout << "error";
		}
	}
}

string Replace_Space_To_(const char* String, int n)					// меняем пробелы на _
{
	string New_String = String;
	int i = 0;
	while (i < n)
	{
		if ((int)New_String[i] == 32) New_String[i] = '_';
		i++;
	}
	return New_String;
}

struct Vertex_AVL* Find_Publisher(string S, struct Vertex_AVL* p)	// поиск издательства
{
	while (p != NULL)
	{
		if (strcmp(S.c_str(), p->Data.Publisher) < 0) p = p->ptr_Left;
		else if (strcmp(S.c_str(), p->Data.Publisher) > 0) p = p->ptr_Right;
		else break;
	}
	if (p == NULL)
	{
		SET_CP_OFF; cout << "\n " << S;
		SET_CP_ON;  cout << " - Нет такого издательства";
	}
	return p;
}

void Find_Autor(struct Vertex_AVL* p, string S)						// поиск книг авторов
{
	set <string> SET;
	multimap <string, struct Vertex_AVL*> Map;

	while (p != 0)
	{
		if (strstr(p->Data.Author, S.c_str()))
			Map.insert(make_pair(p->Data.Author, p));
		p = p->Next;
	}
	if (Map.empty())
	{
		SET_CP_OFF; cout << "\n " + S;
		SET_CP_ON;  cout << " - Нет такого автора";
	}
	else
	{
		SET_CP_ON;  cout << "\n" << setw(8) << "\tАвтор:" << setw(28) << "Название книги:"
			<< setw(20) << "Страниц" << setw(5) << "Год" << endl;

		int i = 1;
		for (auto it = Map.begin(); it != Map.end(); it++, i++)
		{
			SET_CP_OFF; cout << "\n " << i << ".\t" + Replace_Space_To_(it->first.c_str(), it->first.length())
				<< "  " << Replace_Space_To_(it->second->Data.Title, sizeof(it->second->Data.Title) - 1)
				<< setw(7) << it->second->Data.Pages << setw(8) << it->second->Data.Year;

			SET.insert(it->first);

			if (i % 45 == 0)						// поэкранный вывод изданий
			{
				SET_CP_ON; cout << "\n * Осталось книг: " << Map.size() - i
					<< ". ENTER - продолжить, ESC - выход ";
				if (_getch() == 27)
				{
					i++; break;
				}
			}
		}
		SET_CP_ON; cout << "\n -----------------------------\n" << " Выведено книг: " << i - 1
			<< ", Авторов: " << SET.size();
	}
}

int Count_Book(struct Vertex_AVL* p)				// количество книг в издательстве
{
	int n = 1;
	while (p->Next != NULL)
	{
		p = p->Next;
		n++;
	}
	return n;
}

void Publisher_And_Books(struct Vertex_AVL* p)	// вывод всех издательств
{
	if (p != NULL)
	{
		Publisher_And_Books(p->ptr_Left);
		cout << "\n " << n++ << ". " << Replace_Space_To_(p->Data.Publisher, sizeof(p->Data.Publisher)) << "\t" << Count_Book(p);
		Publisher_And_Books(p->ptr_Right);
	}
}

void Load_Case(int Key)							// поиск по выбору пользователя
{
	do
	{
		system("cls");

		if (Key == 1)				   // вывод издательств и количество книг	
		{
			cout << "\n     Издательства  " << "\tКол-во книг" << endl;
			SET_CP_OFF;
			Publisher_And_Books(Root_AVL);
			n = 1;
		}
		else	                           // поиск книг автора в издательстве
		{
			system("mode con lines=48");
			string Author;
			string Publisher;

			cout << "\n Введите название издательства: "; getline(cin, Publisher);

			if (Publisher.length() < size(Root_AVL->Data.Publisher))				// добавляем к строке пробелы
			{
				int delta = size(Root_AVL->Data.Publisher) - Publisher.length() - 1;
				string Space(delta, char(32));

				Publisher = Publisher + Space;
			}
			struct Vertex_AVL* rezult = Find_Publisher(Publisher, Root_AVL);

			if(rezult != NULL)
			{
				SET_CP_ON; cout << "\n Введите автора: ";
				getline(cin, Author);
				Find_Autor(rezult, Author);
			}
		}
		SET_CP_ON;  cout << "\n\n ENTER - повторить, ESC - выход ";

	} while (_getch() != 27);
}

void Add_Null_To_String(char String[], int n)	// задаем формат строки
{
	int i = 0;
	String[n - 1] = '\0';
}

int size_wood(struct Vertex_AVL* p)				// размер дерева
{
	if (p == NULL)
		return 0;
	else
		return 1 + size_wood(p->ptr_Left) + size_wood(p->ptr_Right);
}

int height_wood(struct Vertex_AVL* p)			// высота дерева
{
	if (p == NULL)
		return 0;
	else
		return 1 + max(height_wood(p->ptr_Left), height_wood(p->ptr_Right));
}

/*Функция выполняет однократный поворот между узлом и его левым ребенком,
 корректирует высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* LL(struct Vertex_AVL* p)					// LL поворот
{
	struct Vertex_AVL* q = p->ptr_Left;
	p->ptr_Left = q->ptr_Right;
	q->ptr_Right = p;

	p->Balance = height_wood(p);							// корректировка высот
	q->Balance = height_wood(q);

	return q;
}
/*Функция выполняет однократный поворот между узлом и его правым ребенком,
 корректирует высоты поддеревьев, после чего возвращает новый корень*/
struct Vertex_AVL* RR(struct Vertex_AVL* p)					// RR поворот
{
	struct Vertex_AVL* q = p->ptr_Right;
	p->ptr_Right = q->ptr_Left;
	q->ptr_Left = p;

	p->Balance = height_wood(p);							// корректировка высот
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

struct Vertex_AVL* AVL(struct AVL_Node & New, struct Vertex_AVL *p)		// создание АВЛ
{
	if (p == NULL)
	{
		p = new Vertex_AVL();
		p->Data = New;
		p->Next = NULL;
		p->ptr_Left = NULL;
		p->ptr_Right = NULL;
		p->Balance = 0;
	}
	else if (strcmp(New.Publisher, p->Data.Publisher) < 0)
	{
		p->ptr_Left = AVL(New, p->ptr_Left);
		if (height_wood(p->ptr_Left) - height_wood(p->ptr_Right) == 2)
			if (strcmp(New.Publisher, p->ptr_Left->Data.Publisher) < 0)
				p = LL(p);
			else
				p = LR(p);
	}
	else if (strcmp(New.Publisher, p->Data.Publisher) > 0)
	{
		p->ptr_Right = AVL(New, p->ptr_Right);
		if (height_wood(p->ptr_Right) - height_wood(p->ptr_Left) == 2)
			if (strcmp(New.Publisher, p->ptr_Right->Data.Publisher) > 0)
				p = RR(p);
			else
				p = RL(p);
	}
	else if ((strcmp(New.Publisher, p->Data.Publisher) == 0))
	{
		struct Vertex_AVL* Temp = p;
		while (Temp->Next != NULL) Temp = Temp->Next;
		Temp->Next = AVL(New, Temp->Next);
	}
	return p;
}

void AddNull(char *String1, char *String2, int n)		
{
	for (int i = 0; i < n; i++)
		String2[i] = String1[i];
	
	String2[n] = '\0';									// добавляем символ конца строки
}

int DB_Load()
{
	FILE* fp;

	if ((fp = fopen("BASE1.DAT", "rb")) == NULL)
	{
		SET_CP_ON;
		perror("\n ОШИБКА");
		cout << "\n Программа завершена с шибкой!\n";
		Sleep(5000);
		exit(1);
	}
	else
	{
		while (!feof(fp))
		{
			fread(&New_Value, sizeof(struct DB_Read), 1, fp);

			AddNull(New_Value.Author, New_ValueNew.Author, size(New_Value.Author));
			AddNull(New_Value.Title, New_ValueNew.Title, size(New_Value.Title));
			AddNull(New_Value.Publisher, New_ValueNew.Publisher, size(New_Value.Publisher));
			New_ValueNew.Year = New_Value.Year;
			New_ValueNew.Pages = New_Value.Pages;

			Root_AVL = AVL(New_ValueNew, Root_AVL);
		}
		fclose(fp);
	}
}
