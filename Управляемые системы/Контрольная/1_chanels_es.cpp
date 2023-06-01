#include <ctime>
#define MAXCOLOR16
#include "wingraph.h"
							  
int Pack[] = {495,20,535,20,535,39,495,39,495,20};
int production[] = {540,50,550,60,560,60,550,50,600,50,600,40,460,
					40,460,50,480,50,470,60,480,60,490,50,540,50};

// ------------------------------------------------- ПРОИЗВОДСТВО ТОПЛИВА
void A(){
	int volume, check;
	unsigned long foo;
	
	HANDLE AB = CreateNamedPipe("\\\\.\\pipe\\chanels_es1", PIPE_ACCESS_DUPLEX,			//сервер А создает канал
								PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,1,4,4,0,0);
								
	ConnectNamedPipe(AB, 0);		// ожидаем подключение клиента

	srand(time(0));
	while(1){
		volume = rand()%10 + 4;
		delay(volume*300);
		fillpoly(5,Pack,LIGHTMAGENTA);
		delay(500);

		check = TransactNamedPipe(AB,&volume,4,&foo,4,&foo,0);	// отправляем в канал chanels_es1 объем топлива
																// и ожидаем сообщения от нити В об его приеме
		fillpoly(5,Pack,15);
		if(!check) break;	// 	если клиент закрыл соединение, завершаем процесс												
		
	}
		ExitProcess(0);		// завершение в случае отсоединения клиента	
}

void mainx(){
	resize(600, 100);
	
	setfillstyle(1,YELLOW);
	fillpoly(13,production,0);
	setfillstyle(1,15);
	
	A();					// запускаем производство топлива		
}
