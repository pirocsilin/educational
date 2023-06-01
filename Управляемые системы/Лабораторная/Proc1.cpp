#define MAXCOLOR16
#include "wingraph.h"

using namespace std;
 
struct SharedData{
	int dy1,dy2,dy3,dy4;
	int X, Y;
	int stop;
}*p;

enum TypeFig{
	LEFT_CIRCLE  = 1,
	RIGHT_CIRCLE = 2,
	LEFT_SQUARE  = 3,
	RIGHT_SQUARE = 4
};

struct FigData{
	int x, y, d;
	TypeFig Type;
};

char XY[10];
const int rad = 50;
int SqColor = BLUE;
bool StopThread4 = 0;

void MoveFigure(FigData *ptr){
	switch(ptr->Type){
		case 1:
				int Y1;
				for (int i = 1; i < p->X-2*rad-8; i++){
					Y1 = ptr->y - p->dy1;
					circle (ptr->x + i, Y1, rad, 1);
					delay(ptr->d);
					circle (ptr->x + i, Y1, rad, 15);
					if(ptr->x+i >= 320){
						SqColor = RED;
						StopThread4 = 1;
					}
				}
				circle (p->X-rad-8, Y1, rad, 1);
				break;
		case 2:
				int Y2;
				for (int i = 1; i < p->X-2*rad; i++){
					Y2 = ptr->y - p->dy2;
					circle (ptr->x - i, Y2, rad, 1);
					delay(ptr->d);
					circle (ptr->x - i, Y2, rad, 15);	
				}
				circle (rad, Y2, rad, 1);
				break;
		case 3:
				int Y3;
				for (int i = 1; i < p->X-2*rad-8; i++){
					Y3 = ptr->y - p->dy3;
					rectangle(ptr->x + i, Y3, ptr->x+i+2*rad, Y3+2*rad,SqColor);
					delay(ptr->d);
					rectangle(ptr->x + i, Y3, ptr->x+i+2*rad, Y3+2*rad,15);
				}
				rectangle(p->X-2*rad-8, Y3, p->X-8, Y3+2*rad,SqColor);
				break;
		case 4:
			    int Y4;
				for (int i = 1; i < p->X-2*rad; i++){
					Y4 = ptr->y - p->dy4;
					rectangle(ptr->x - i, Y4, ptr->x-i+2*rad, Y4+2*rad,SqColor);
					delay(ptr->d);
					rectangle(ptr->x - i, Y4, ptr->x-i+2*rad, Y4+2*rad,15);
				}
				rectangle(0, Y4, 2*rad, Y4+2*rad,SqColor);
				while(!StopThread4){delay(20);}
				rectangle(0, Y4, 2*rad, Y4+2*rad,SqColor);
				break;
	}
}

void CheckedStopProc(HANDLE hMap){
	while(1){
		if(p->stop){
			UnmapViewOfFile(p);	// удаление ранее созданных ресурсов
			CloseHandle(hMap);
			ExitProcess(0);		// Завершение процесса
		}
		delay(1000);
	}
}

int SetY(int dY, int delta) { return delta*dY-dY/2-10; }

void mainx ()
{
	char str[40];			 //Массив для помещения значения из переменной среды
  
	HANDLE hMap;			 					//Описатель, куда будем записывать знач из переменной среды
	if(GetEnvironmentVariable("hMap",str,40))   //если есть разделяемая переменная 'hMap', то..
		hMap = (HANDLE)atoi(str);
	else
		ExitProcess(1);							//иначе, Завершаем процесс с ошибкой - 1
  	
	// отображаем объект в оперативную память
	p = (SharedData*) MapViewOfFile(hMap,					// описатель созданного объекта
  	 								FILE_MAP_WRITE,	   		// уровень доступа (чтение, запись)
  									0,0,				  	// 0 - смещение
  									sizeof(SharedData));	// отображаем весь объект целиком						  
	if(p == 0) ExitProcess(2);								//иначе, Завершаем процесс с ошибкой - 2						  
  	
	if(p->X > 1000 || p->X < 640) p->X = 640;
	if(p->Y > 1000 || p->Y < 480) p->Y = 480;
	
	resize(p->X, p->Y);
	
	int dY = p->Y / 4;
	
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) CheckedStopProc, (LPVOID) hMap, 0, 0);
	
	FigData *ptr;
	ptr = new FigData{rad,SetY(dY,1),100,LEFT_CIRCLE};
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) MoveFigure, (LPVOID) ptr, 0, 0);
	
	ptr = new FigData{p->X-rad,SetY(dY,2),100,RIGHT_CIRCLE};
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) MoveFigure, (LPVOID) ptr, 0, 0);
	
	ptr = new FigData{0,SetY(dY,3)-dY/2,50,LEFT_SQUARE};
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) MoveFigure, (LPVOID) ptr, 0, 0);
	
	ptr = new FigData{p->X-2*rad,SetY(dY,4)-dY/2,25,RIGHT_SQUARE};
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) MoveFigure, (LPVOID) ptr, 0, 0);
}
