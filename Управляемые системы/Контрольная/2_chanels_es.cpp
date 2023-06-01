#include <string>
#include <ctime>
#define MAXCOLOR16
#include "wingraph.h"

using namespace std;

enum Direction{
	DOWN = 1,
	MOVETO = 2,
	BACK = 3
};

void B();
void B1(int);

int cart[] = {490,23,490,30,540,30,540,23,490,23};
int MaketPack[] = {495,20,535,20,535,39,495,39,495,20};
int pack[10];

void BurnGas(int n, int volume){
	const int Xrad = 23, Yrad = 5;
	int x,y;
	delay(500);
	y = 267;
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
	while(y <= 267){
		fillellipse(x,y,Xrad,Yrad,15);
		y += 14;
		delay(1500);
	}
}

void MovePack(Direction dir, int pos=0){
	switch (dir){
		case 1: for(int i=0; i<sizeof(pack)/sizeof(int); i++)
					pack[i] = MaketPack[i];
				delay(200);
				for(int i=0; i<10; i++){
				if(i%2!=0) pack[i] -= 20;
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
		case 3: fillpoly(5,pack,15);
				delay(200);
				for(int i=0; i<10; i++){
					if(i%2!=0) pack[i] += 80;
				}
				fillpoly(5,pack,LIGHTMAGENTA);
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
// -------------------------------------------------
void B(){
	HANDLE BB1[4];
	int volume, n;
	unsigned long foo;
						  
	HANDLE AB = CreateFile("\\\\.\\pipe\\chanels_es1", GENERIC_READ | GENERIC_WRITE, // клиент подключается
							   0,0, OPEN_EXISTING, 0,0);							 // к каналу chanels_es1
							   
	// сервер B создает i-й экземпл¤р канала chanels_es2
	// после создаетс¤ i-й клиент ¬1 который подключаетс¤ к каналу
	for(int i=0; i<4; i++){
		BB1[i] = CreateNamedPipe("\\\\.\\pipe\\chanels_es2", PIPE_ACCESS_DUPLEX,
								  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,4,4,4,0,0);
								  
		CreateThread(0,0, (LPTHREAD_START_ROUTINE) B1, (LPVOID) (i+1), 0, 0);					  
	}

	while(1){
		int msg=0;
		
		ReadFile(AB, &volume, 4, &foo, 0);	// ожидаем сообщение в канале о готовности топлива
		
		if(foo==0) break; 					// если прочитано 0 байт, получили ошибку, завершаем процесс
		
		MovePack(DOWN);
 		WriteFile(AB, &foo, 4, &foo, 0);   	// сообщение в канал о приеме топлива
		
		// провер¤ем наличие сообщени¤ в каждом экземпл¤ре канала
		while(!msg){

			for(int j=0; j<4; j++){
				PeekNamedPipe(BB1[j],&msg,4,0,0,0);
				if(msg)break;
			}
			string str = to_string(msg);
			outtextxy(50,100,(char*)str.c_str());
		}
		ReadFile(BB1[msg-1], &n, 4, &foo, 0);		// читаем сообщение о необходимости топлива	
		
		MovePack(MOVETO,n);							// доставл¤ем топливо в n-ый котел

		WriteFile(BB1[msg-1], &volume, 4, &foo, 0);	// сообщаем вес топлива

		MovePack(BACK);								// выгрузка топлива и возврат тележки в крайнее положение
	}
	ExitProcess(0);
}
// -------------------------------------------------
void B1(int n){
	int volume;
	unsigned long foo;
	MSG msg;
	

	HANDLE BB1 = CreateFile("\\\\.\\pipe\\chanels_es2", GENERIC_READ | GENERIC_WRITE, // клиент B1 подключается
							   0,0, OPEN_EXISTING, 0,0);							  // к каналу chanels_es2
	while(1){
		
		WriteFile(BB1, &n, 4, &foo, 0);			// сообщаем в канал о необходимости топлива передав номер котла
		ReadFile(BB1, &volume, 4, &foo, 0); 	// ожидаем сообщени¤ в канале о передаче топлива

		BurnGas(n, volume);						// сжигаем топливо
	}
}

void Area(){
	int frame[] = {100,100,100,275,150,275,150,100};
	int bunker[] = {90,50,100,60,150,60,160,50,90,50};
	int conveyor[] = {50,45,45,43,45,40,50,38,550,38,
						555,40,555,43,550,45,50,45};
	
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
}

void mainx(){
	resize(600, 300);
	
	Area();							// рисуем статические объекты
	setfillstyle(1,15);
	fillpoly(5,cart,LIGHTMAGENTA);	// рисуем тележку
	
	B();							// запускаем транспортер
}
