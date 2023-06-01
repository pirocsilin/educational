#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

struct SharedData{				// Структура для размещения в разделяемую память процессов
	int dy1,dy2,dy3,dy4;
	int X, Y;
	int stop;
};

char* NameProc = "Proc1.exe";

void StopProc(HANDLE hMap, SharedData *p){
	p->stop = 1; 						// Сигнал для завершение дочернего процесса
    UnmapViewOfFile(p);					// Удаление ранее созданных ресурсов
    CloseHandle(hMap);
    ExitProcess(0);						// Завершение процесса
}
 
int main(int argc, char *argv[])
{
	setlocale (LC_ALL, "Russian");
	SetConsoleCP (1251);
	SetConsoleOutputCP(1251);
	
	SECURITY_ATTRIBUTES SA;		 // Параметр 'SA' задает атрибуты защиты объекта
	SA.nLength = sizeof(SA);	 // Размер структуры
	SA.lpSecurityDescriptor = 0; // Стандартные свойства безопасности объект
	SA.bInheritHandle = TRUE;	 // Разрешение наследования описателя (объекта) 'hMap' дочерним проц-м
	
	// создаем объект для отображения файла с описателем hMap
	HANDLE hMap = CreateFileMapping((HANDLE)-1, &SA,
  								  PAGE_READWRITE, 0,
								  sizeof(SharedData),
								  0);	// 0, т.к. анонимный объект (анон обл памяти)
								  
	// отображаем объект в оперативную память
    SharedData *p = (SharedData*) MapViewOfFile(hMap,
  											    FILE_MAP_WRITE,
  											    0,0,
  											    sizeof(SharedData));
	if(argv[1] && argv[2]){
		p->X = atoi(argv[1]);
		p->Y = atoi(argv[2]);
	}
	else{
		p->X = 640;
		p->Y = 480;
	}
	p->dy1 = 0;
	p->dy2 = 0;
	p->dy3 = 0;
	p->dy4 = 0;
	p->stop = 0;
								  
	char str[40];							// Передача описптеля в дочерний проц:
	sprintf(str, "%d", hMap);				// 1. Копируем описатель 'hMap' в 'str'			
	SetEnvironmentVariable("hMap", str);	// 2. Создаем переменную окружения'hMap'					  
	
	STARTUPINFO SI;				// Структура определения визуальных настроек для new проц
	ZeroMemory(&SI, sizeof SI);	// Заполняем блок памяти нулями
	SI.cb = sizeof SI;			// Устанавливаем размер структуры в байтах
	PROCESS_INFORMATION PI;		// Структура содержит инфо о new проц и первичном потоке
								// Создаем новый процесс:
	CreateProcess(NameProc,0,0,0,TRUE,0,0,0,&SI,&PI);

	int *dy;
	while(1){
		system("cls");
		cout<<"\n\n\t1 - Первый круг"<<endl;
		cout<<"\t2 - Второй круг"<<endl;
		cout<<"\t3 - Первый квадрат"<<endl;
		cout<<"\t4 - Второй квадрат"<<endl;
		cout<<"\t5 - Выход"<<endl;
		cout<<"\n\tВыберите фигуру для управления: ";
		
		switch(getche()-'0'){
			case 1: dy = &p->dy1; cout<<"\n\n\tВыбран первый круг:"; break;
			case 2: dy = &p->dy2; cout<<"\n\n\tВыбран второй круг:"; break;
			case 3: dy = &p->dy3; cout<<"\n\n\tВыбран первый квадрат:"; break;
			case 4: dy = &p->dy4; cout<<"\n\n\tВыбран второй квадрат:"; break;
			case 5: StopProc(hMap, p);
			default: continue;
		}
		cout<<"\n\t\t'W' - переместить вверх"<<endl;
		cout<<"\t\t'S' - переместить вниз"<<endl;
		cout<<"\t\t'N' - выбрать другую фигуру"<<endl;
		cout<<"\t\t'Q' - Выход ";
		
		int ExitCode = 0;
		while(!ExitCode){
			switch(_getch()){
				case 'w':
				case 'W': *dy += 10; break;
				case 's': 
				case 'S': *dy -= 10; break;
				case 'n': 
				case 'N': ExitCode = 1; break;
				case 'q': 
				case 'Q': StopProc(hMap, p);
			}
		}
	}
	return 0;
}
