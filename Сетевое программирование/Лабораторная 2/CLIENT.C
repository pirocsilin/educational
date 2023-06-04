//-------------------------------------------------------
//				    		CLIENT
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

unsigned char in_buffer[MAXDIR];
unsigned char out_buffer[] = "Request directory and FIO";
unsigned short NumSocket = 0x0000;
unsigned short ServerSocket = 0x4444;	// Сокет сервера
int i;

void OpenSocket(){
    ipxgetaddress(&myaddress);
    myaddress.socket=NumSocket;			// Динамическое получение сокета
										// для клиента из числа свободных
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
	memset(recb, 0, sizeof(recb));
    recb.socket=myaddress.socket;
    recb.esraddress=NULL;
    recb.fragcount=2;
    recb.fragaddr1=&rheader;
    recb.fragsize1=sizeof(IPXHEADER);
    recb.fragaddr2=in_buffer;
    recb.fragsize2=sizeof(in_buffer);
}
 void InitSendECB(){
	memset(secb, 0, sizeof(secb));
    secb.socket=myaddress.socket;
									// Сообщение будет отправлено на все узлы
									// сети которые работают на сокете № 0x4444
    for(i=0;i<6;i++) secb.immedaddr[i]=0xFF;	
    secb.esraddress=NULL;
    secb.fragcount=2;
    secb.fragaddr1=&sheader;
    secb.fragsize1=sizeof(IPXHEADER);
    secb.fragaddr2=out_buffer;
    secb.fragsize2=sizeof(out_buffer);
}
void SetSendHeader(){
									// "Широковещательный" запрос на все узлы
									// сети которые работают на сокете № 0x4444
    for(i=0;i<4;i++) sheader.dest.netadd[i]=0x00;
    for(i=0;i<6;i++) sheader.dest.nodeadd[i]=0xFF;
    sheader.dest.socket=ServerSocket;
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

    if(!ipxinit())
    {
        printf("IPX not installed\n");
        exit(-1);
    }

    printf("\n%45s\n", ">> CLIENT <<");
    OpenSocket();
	
    SetSendHeader();
    InitSendECB();
    printf("\n Press any key to send request ");
    getch();
    ipxsendpacket(&secb);
	
    InitResivECB();
    ipxlistenforpacket(&recb);

    printf("\n\n Waiting response from server...");
    printf("\n or any key - EXIT\n");

    while(recb.inuse){
		ipxrelenquishcontrol();
		if(kbhit()){
			getch();
			recb.cc = 0xFC;
			Close();
			}
    }

    printf("\n Accept response from server: %s", in_buffer);
    Close();
}
