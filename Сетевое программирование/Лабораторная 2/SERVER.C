//-------------------------------------------------------
//				    		SERVER
//-------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include <dir.h>
#include "ipx.h"

IPXADDRESS myaddress;

IPXECB recb;
IPXECB secb;

IPXHEADER rheader;
IPXHEADER sheader;

unsigned char in_buffer[200], out_buffer[MAXDIR];
const char *name = "\\Tryushnikov Vadim Georgievich";
unsigned short NumSocket = 0x4444;		// Сокет на котором запускаем сервер
int i;

void OpenSocket(){
    myaddress.socket=NumSocket;
    switch(ipxopensocket(0x00,&myaddress.socket))
    {
        case 0x00:
			printf(" Socket %X is open\n",reverseword(myaddress.socket));
			break;
        case 0xFE:
            printf(" Socket Table Full\n");
            exit(1);
        case 0xFF:
            printf(" Socket Already Open\n");
            exit(1);
    }
}
void InitResivECB(){
	memset(recb, 0, sizeof(recb));		// Очистка блока (заполнение нулями)
    recb.socket=myaddress.socket;		// Сокет, на котором принимаем пакет	
    recb.esraddress=NULL;				// Нет процедуры обработки по завершении приема 
    recb.fragcount=2;					// Количество блоков для приема
    recb.fragaddr1=&rheader;			// Адрес заголовка для приема 
    recb.fragsize1=sizeof(IPXHEADER);	// Размер заголовка приема
    recb.fragaddr2=in_buffer;			// Адрес буфера для приема данных
    recb.fragsize2=sizeof(in_buffer);	// Размер буфера
}
 void InitSendECB(){
	memset(secb, 0, sizeof(secb));
    secb.socket=myaddress.socket;				// Сокет, с которого отправляем пакет
	memcpy(secb.immedaddr, recb.immedaddr, 6);	// Установка адреса назначения пакета
    secb.esraddress=NULL;
    secb.fragcount=2;
    secb.fragaddr1=&sheader;
    secb.fragsize1=sizeof(IPXHEADER);
    secb.fragaddr2=out_buffer;
    secb.fragsize2=sizeof(out_buffer);
}
void SetSendHeader(){
	memset(sheader.dest.netadd, 0, 4);				// Установка сети назначения 
    memcpy(sheader.dest.nodeadd, recb.immedaddr, 6);// Установка узла назначения
    sheader.dest.socket=rheader.source.socket;		// Устновка сокета назначения 
    sheader.type=4;
}

void Close(){
    printf("\n\n Program exit\n");
    ipxclosesocket(myaddress.socket);
    exit(1);
}

void main ( void )
{
    system("cls");

    if(!ipxinit())						// Проверяем наличия IPX и определяем точку входа
    {
        printf("IPX not installed\n");
        exit(1);
    }

    printf("\n%45s\n", ">> SERVER <<");
    OpenSocket();						// Открываем сокет сервера
    InitResivECB();						// Подготовка ECB для приема данных
    ipxlistenforpacket(&recb);			// Ожидаем получение пакета от клиента

    printf("\n Waiting for Request...");
    printf("\n or any key - EXIT\n");

    while(recb.inuse){			// Проверяем флаг окончания приема в recb
	ipxrelenquishcontrol();		
	if(kbhit()){				// Если нажали клавишу, завершение программы						
	    getch();
	    recb.cc = 0xFC;
	    Close();
        }
    }
								// Вывод принятого пакета и № сокета клиента
    printf("\n Accept request from Socket[%X]: %s", 
			reverseword(rheader.source.socket), in_buffer);
    printf("\n\n Press any key to send a reply");
    getch();					

    SetSendHeader();			// Подготовка блока secb для отправки данных
    InitSendECB();				// Подготовка заголовка для отправки данных
	
	getcwd(out_buffer, sizeof(out_buffer));	// Подготовка данных для передачи
	strcat(out_buffer, name);

    ipxsendpacket(&secb);					// Отправка пакета данных клиенту
	printf("\n\n Reply send to node: ");
    for(i=0;i<6;i++) printf("%02X ", secb.immedaddr[i]);
    Close();
}
