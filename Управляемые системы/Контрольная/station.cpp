#include <ctime>
#define MAXCOLOR16
#include "wingraph.h"

enum Direction{
	DOWN = 1,
	MOVETO = 2,
	BACK = 3
};

int cart[] = {490,123,490,130,540,130,540,123,490,123};

class Station{
	public:
		int x, y;
		static const int Xrad = 23, Yrad = 5;
		int pack[10] = {495,20,535,20,535,39,495,39,495,20};
		static int frame[], bunker[], conveyor[], production[];
		
		Station(char *p){
			fillpoly(5,pack,LIGHTMAGENTA);
		}
		
		Station(){
			setfillstyle(1,LIGHTGRAY);
			for(int j=0; j<4; j++){
				drawpoly(4,frame,0);
				fillpoly(5,bunker,0);
				for(int i=0; i<8; i++)
					if(i%2==0) frame[i] += 100;
				for(int i=0; i<10; i++)
					if(i%2==0) bunker[i] += 100;
			}
			setfillstyle(1,BLACK);
			fillpoly(9,conveyor,0);
			setfillstyle(1,YELLOW);
			fillpoly(13,production,0);
		};
		
		Station(int n, int volume){
			delay(500);
			y = 367;
			volume = y - volume * 14;
			switch (n){
				case 1: x = 125; break;
				case 2: x = 225; break;
				case 3: x = 325; break;
				case 4: x = 425; break;
			}
			while(y > volume){
				fillellipse(x,y,Xrad,Yrad,LIGHTMAGENTA);
				y -= 14;
			}
			while(y <= 367){
				fillellipse(x,y,Xrad,Yrad,15);
				y += 14;
				delay(1500);
			}
		};
		
		void MovePack(Direction dir, int pos=0){
			switch (dir){
				case 1: fillpoly(5,pack,15);
						delay(200);
						for(int i=0; i<10; i++){
							if(i%2!=0) pack[i] += 80;
						}
						fillpoly(5,pack,LIGHTMAGENTA);
						break;
				case 2: pos *= 100;
						do{
							fillpoly(5,pack,15);
							fillpoly(5,cart,15);
							for(int i=0; i<10; i++){
								if(i%2==0){
									cart[i] -= 4;
									pack[i] -= 4;
								}
							}
							fillpoly(5,cart,LIGHTMAGENTA);
							fillpoly(5,pack,LIGHTMAGENTA);
							delay(1);
						}while(cart[0]>pos);
						break;
				case 3: MovePack(DOWN);
						delay(200);
						fillpoly(5,pack,15);
						do{
							fillpoly(5,cart,15);
							for(int i=0; i<10; i++){
								if(i%2==0){
									cart[i] += 4;
								}
							}
							fillpoly(5,cart,LIGHTMAGENTA);
							delay(1);
						}while(cart[0]<490);
						
			}
		}
}*p;

int Station:: frame[] = {100,200,100,375,150,375,150,200};
int Station:: bunker[] = {90,150,100,160,150,160,160,150,90,150};
int Station:: conveyor[] = {50,145,45,143,45,140,50,138,550,138,
							555,140,555,143,550,145,50,145};
int Station:: production[] = {540,50,550,60,560,60,550,50,600,50,600,40,460,
							  40,460,50,480,50,470,60,480,60,490,50,540,50};
						  
unsigned long idA, idB[5];

// -------------------------------------------------
void A(){
	int volume;
	MSG msg;
	PeekMessage(&msg,0,0,0,PM_NOREMOVE);
	srand(time(0));
	while(1){
		volume = rand()%10 + 4;							// объем партии топлива (от 4 до 13 у.е.)
		delay(volume*300);								// врем¤ производства
		Station Pack = Station("pack");					// произвели топливо
		delay(500);
		p = &Pack;
		
		PostThreadMessage(idB[0], WM_USER, volume, 0);	// сообщение транспортеру о готовности топлива
		GetMessage(&msg, 0, 0, 0);						// ожидаем сообщение о приеме топлива
	}
}
// -------------------------------------------------
void B(){
	int volume;
	MSG msg;
	PeekMessage(&msg,0,0,0,PM_NOREMOVE);
	while(1){
		GetMessage(&msg, 0, WM_USER, WM_USER);			// ожидаем производства товлива
		volume = msg.wParam;							// получаем объем
		Station Pack = *p;								// получаем пакет с топливом
		Pack.MovePack(DOWN);							// перемещаем на тележку
		PostThreadMessage(idA, WM_USER, 0, 0);			// отправл¤ем сообщение о получении
		
		GetMessage(&msg,0,WM_USER+1,WM_USER+4);			// ожидаем запрос топлива от n-го котла
		
		int n = msg.wParam;								// получаем номер котла из прин¤того сообщени¤

		Pack.MovePack(MOVETO,n);						// доставл¤ем топливо в n-ый котел
		
		PostThreadMessage(idB[n], WM_USER, volume, 0);	// отправл¤ем n-му котлу сообщение об оЏбъеме топлива

		Pack.MovePack(BACK);							// выгрузка топлива и возврат тележки в крайнее положение
	}
}
// -------------------------------------------------
void B1(int n){
	int volume;
	MSG msg;
	PeekMessage(&msg,0,0,0,PM_NOREMOVE);
	while(1){
		PostThreadMessage(idB[0], WM_USER+n, n, 0);		// сообщаем транспортеру требование на топливо
		GetMessage(&msg,0,0,0);							// и ожидаем его получени¤ (важен параметр, код не важен)
		volume = msg.wParam;
		
		Station Reaction = Station(n, volume);			// сжигаем топливо
	}
}

void mainx(){
	resize(600, 400);
	
	Station Area = Station();												// рисуем статические объекты
	setfillstyle(1,15);
	fillpoly(5,cart,LIGHTMAGENTA);											// рисуем тележку
	
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) A, 0, 0, &idA);				// запускаем производство топлива
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) B, 0, 0, idB + 0);			// запускаем транспортер
	
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) B1, (LPVOID) 1, 0, idB + 1); // котел 1
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) B1, (LPVOID) 2, 0, idB + 2); // котел 2
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) B1, (LPVOID) 3, 0, idB + 3); // котел 3
	CreateThread(0,0, (LPTHREAD_START_ROUTINE) B1, (LPVOID) 4, 0, idB + 4); // котел 4
}
