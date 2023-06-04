#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <direct.h>
#define DEFAULT_PORT "27000"
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFLEN 512
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const char* name = "\\Tryushnikov Vadim Georgievich";
char SendBuf[DEFAULT_BUFLEN];
char RecvBuf[DEFAULT_BUFLEN];
int RecvBufLen = DEFAULT_BUFLEN;
int Result;

void InitWinSock() {
    WSADATA wsaData;
    Result = WSAStartup(MAKEWORD(2, 0), &wsaData);
    if (Result != 0) {
        printf("\n Ошибка инициализации WinSock: %d\n", Result);
        WSACleanup();
        exit(-1);
    }
}

addrinfo* InitAddrInfo(bool flags = false) {
    addrinfo* result, hints;
    ZeroMemory(&hints, sizeof(hints));

    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (flags) {
        hints.ai_family = AF_INET;
        hints.ai_flags = AI_PASSIVE;
        Result = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    }
    else {
        hints.ai_family = AF_UNSPEC;
        Result = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
    }
    if (Result != 0) {
        printf("\n Ошибка инициализации getaddrinfo: %d", Result);
        WSACleanup();
        exit(-1);
    }
    return result;
}

SOCKET CreateSocket(addrinfo *p) {
    SOCKET Socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (Socket == INVALID_SOCKET) {
        printf("\n Ошибка инициализации Сокета: %d", WSAGetLastError());
        freeaddrinfo(p);
        WSACleanup();
        exit(-1);
    }
    return Socket;
}

 bool ConnectToServer(SOCKET &s, addrinfo* p) {
    Result = connect(s, p->ai_addr, (int)p->ai_addrlen);
    if (Result == SOCKET_ERROR) {
        freeaddrinfo(p);
        closesocket(s);
        return false;
    }
    freeaddrinfo(p);
    return true;
}

 void BindSockAndAddr(SOCKET &s, addrinfo* p) {
     Result = bind(s, p->ai_addr, (int)p->ai_addrlen);
     if (Result == SOCKET_ERROR) {
         printf("\n Bind failed with error: %d", WSAGetLastError());
         freeaddrinfo(p);
         closesocket(s);
         WSACleanup();
         exit(-1);
     }
     freeaddrinfo(p);
 }

 void SendMsg(SOCKET &ConnectSocket) {
     printf("\n %40s", ">> CLIENT <<");

     _getcwd(SendBuf, sizeof(SendBuf));             // Подготовка данных для отправки на сервер
     strcat_s(SendBuf, name);

     printf("\n Отправка данных на сервер..\n");
     Result = send(ConnectSocket, SendBuf, (int)strlen(SendBuf), 0);
     if (Result == SOCKET_ERROR) {
         printf("\n Ошибка отправки данных: %d", WSAGetLastError());
         closesocket(ConnectSocket);
         WSACleanup();
         exit(-1);
     }
     printf("\n Отправлено байт: \t%d", Result);

     Result = shutdown(ConnectSocket, SD_SEND);     // Закрываем передачу данных но все еще
     if (Result == SOCKET_ERROR) {                  // можем принимать их от сервера
         printf("\n Ошибка закрытия ConnectSocket: %d", WSAGetLastError());
         closesocket(ConnectSocket);
         WSACleanup();
         exit(-1);
     }
                                                    // Принимаем данные от сервера
     do {
         Result = recv(ConnectSocket, RecvBuf, RecvBufLen, 0);

         if (Result > 0) printf("\n Получено байт: \t%d", Result);

         else if (Result == 0) break;               // Прием данных завершен

         else {
             printf("\n Ошибка получения данных: %d", WSAGetLastError());
             closesocket(ConnectSocket);
             WSACleanup();
             exit(-1);
         }
     } while (Result > 0);

     printf("\n СООБЩЕНИЕ ОТ СЕРВЕРА: \t%s\n\n [Соединение закрыто]\n", RecvBuf);

     closesocket(ConnectSocket);        // После приема данных, закрываем сокет
     WSACleanup();                      // и освобождаем ресурсы полученные у ОС для работы с WS
 }

 void ListenReguest(SOCKET &ListenSocket) {
     printf("\n %40s", ">> SERVER <<");

     if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
         printf("\n Listen failed with error: %d", WSAGetLastError());
         closesocket(ListenSocket);
         WSACleanup();
         exit(-1);
     }
     printf("\n Ожидание подключения клиента..\n");
 }

 SOCKET CreateClientSock(SOCKET& ListenSocket) {
     SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
     if (ClientSocket == INVALID_SOCKET) {
         printf("\n Accept failed with error: %d", WSAGetLastError());
         closesocket(ClientSocket);
         WSACleanup();
         exit(-1);
     }
     return ClientSocket;
 }

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=70 lines=25");

    addrinfo *result = NULL;
    SOCKET Socket, ClientSocket;                //ConnectSocket or ListenSocket;
    
    InitWinSock();                              // Инициализация WinSock

    result = InitAddrInfo();                    // Инициализация addrinfo для Клиента
    Socket = CreateSocket(result);              // Создаем сокет для Клиента
    if (ConnectToServer(Socket, result))        // Пробуем подключиться к серверу
        SendMsg(Socket);                        // Если подключились, передаем сообщение
    else {                                      // Если сервер еще не существует, то:
        result = InitAddrInfo(true);            // Инициализация addrinfo для Сервера
        Socket = CreateSocket(result);          // Создаем сокет для Сервера
        BindSockAndAddr(Socket, result);        // Связываем сокет с сетевым адресом
        ListenReguest(Socket);                  // Прослушиваем входящие запросы
        ClientSocket = CreateClientSock(Socket);// Сокет для принятия сообщения от клиента

        closesocket(Socket);                    // Закрываем слушающий сокет
        
        do {
            Result = recv(ClientSocket, RecvBuf, RecvBufLen, 0);    // Получаем сообщаениее

            if (Result > 0) printf("\n Получено байт: \t%d", Result);

            else if (Result == 0) break;                            // Данные получены

            else {
                printf("\n Ошибка получения данных: %d", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
        } while (Result > 0);

        printf("\n СООБЩЕНИЕ ОТ КЛИЕНТА: \t%s", RecvBuf);

        _getcwd(SendBuf, sizeof(SendBuf));                          // Подготовка ответа клиенту
        strcat_s(SendBuf, name);
                                                                    // Передаем ответ клиенту
        int SendRes = send(ClientSocket, SendBuf, (int)strlen(SendBuf), 0);
        if (SendRes == SOCKET_ERROR) {
            printf("\n Ошибка отправки данных: %d", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        printf("\n Отправлено байт: \t%d\n", SendRes);

        Result = shutdown(ClientSocket, SD_SEND);
        if (Result == SOCKET_ERROR) {
            printf("\n Ошибка закрытия сокета: %d", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
        printf("\n [Соединение закрыто]\n");

        closesocket(ClientSocket);      // Закрываем Клиентский Сокет
        WSACleanup();                   // Освобождаем ресурсы полученные у ОС для работы с WS
    }

    cout << endl;  system("pause");
    return 0;
}