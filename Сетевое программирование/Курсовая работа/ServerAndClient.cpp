#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <string>
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <direct.h>
#include "dirent.h"

#define DEFAULT_PORT "27000"
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_BUFLEN 512
#pragma comment(lib, "Ws2_32.lib")

using namespace std;

const char* _DIR = "dir", * PUT = "put", * PWD = "pwd", 
          * BYE = "bye", * END_FILE = "end_file", *ERR = "ERROR";

char SendBuf[DEFAULT_BUFLEN];
char RecvBuf[DEFAULT_BUFLEN];
char DirBuff[MAX_PATH];
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

SOCKET CreateSocket(addrinfo* p) {
    SOCKET Socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (Socket == INVALID_SOCKET) {
        printf("\n Ошибка инициализации Сокета: %d", WSAGetLastError());
        freeaddrinfo(p);
        WSACleanup();
        exit(-1);
    }
    return Socket;
}

bool ConnectToServer(SOCKET& s, addrinfo* p) {
    Result = connect(s, p->ai_addr, (int)p->ai_addrlen);
    if (Result == SOCKET_ERROR) {
        freeaddrinfo(p);
        closesocket(s);
        return false;
    }
    freeaddrinfo(p);
    return true;
}

void BindSockAndAddr(SOCKET& s, addrinfo* p) {
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

void Server(SOCKET& ClientSocket) {
    LPDWORD n(0);       // Количество фактически записанных байт в файл
    char EHO[1]{};      // Буфер для отправки или получения эха-отклика отклиенту

    while (true) {

        memset(RecvBuf, '\0', sizeof(RecvBuf));                     // Очистка приемного буфера
        Result = recv(ClientSocket, RecvBuf, RecvBufLen, 0);        // Ожидание сообщения от клиента

        if (Result == 0) break;                                     // Если клиент закрыл соединение, 
                                                                    // то завершаем программу
        if (!_strnicmp(RecvBuf, PUT, 3)) {                          // Если пришла команда 'put'

            wstring fName(strlen(RecvBuf + 4), L'\0');              // Подготовка строки wstring под имя файла
            mbstowcs(&fName[0], RecvBuf + 4, strlen(RecvBuf + 4));  // Преобразуем char -> wchar_t = fName - имя файла
                                                                    
            HANDLE hFile = CreateFile(&fName[0], GENERIC_WRITE, 0, NULL,    // Создаем файл для записи
                                      CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                send(ClientSocket, EHO, 1, 0);                          // Сообщаем клиенту о готовности принять файл
                Result = recv(ClientSocket, RecvBuf, RecvBufLen, 0);

                while (_stricmp(RecvBuf, END_FILE)) {                   // Проверяем флаг окончания передачи    

                    WriteFile(hFile, RecvBuf, Result, n, NULL);         // Записываем в файл символы из приемного буфера

                    send(ClientSocket, EHO, 1, 0);                      // Сигнал серверу что очередная порция байт прочитана
                    memset(RecvBuf, '\0', sizeof(RecvBuf));             // Очистка буфера перед очередным приемом сообщения
                    Result = recv(ClientSocket, RecvBuf, RecvBufLen, 0);
                }
                CloseHandle(hFile);                                     // По окончании, закрываем описатель файла
                wcout << "\n Получен файл: " << fName;
            }
            else                                                        // Если не удалось создать файл, передаем клиенту
                send(ClientSocket, ERR, strlen(ERR), 0);                // в TCP канал строку 'ERROR'
        }
        else if (!_stricmp(RecvBuf, PWD)) {                             // Если пришла команда 'pwd' 
            cout << "\n Запрос клиента: " << PWD;
            _getcwd(SendBuf, sizeof(SendBuf));                          // Получаем адрес папки сервера
            send(ClientSocket, SendBuf, (int)strlen(SendBuf), 0);       // и передаем клиенту
        }
        else if (!_stricmp(RecvBuf, _DIR)) {                            // Если пришла команда 'dir'
            cout << "\n Запрос клиента: " << _DIR;
            DIR* dir = opendir(_getcwd(DirBuff, MAX_PATH));             // Открываем "директорный" поток
            dirent* fi;                                                 // Структура для хранения данных файлов
            if (dir) {
                while ((fi = readdir(dir)) != NULL) {                       // Получаем очередную запись о файле
                    send(ClientSocket, fi->d_name, strlen(fi->d_name), 0);  // и отправляем имя файла в канал TCP
                    recv(ClientSocket, EHO, 1, 0);                          // Ожидаем получения клиентом
                }
                send(ClientSocket, END_FILE, strlen(END_FILE), 0);
                closedir(dir);                                          // Закрываем "директорный" поток
            }
            else                                                        // Если "директорный" поток открыть не удалось
                send(ClientSocket, ERR, strlen(ERR), 0);                // отправляем сообщение об ошибке в канал TCP
        }
    }

    printf("\n\n [Соединение закрыто]\n");
    closesocket(ClientSocket);
    WSACleanup();
}

void Client(SOCKET& ConnectSocket) {
    printf("\n %40s", ">> CLIENT <<");
    FILE* f;
    char EHO[1];

    while (true) {
        cout << "\n\n Введите команду: ";

        memset(SendBuf, '\0', sizeof(SendBuf));                     // Очистка буферов SendBuf и RecvBuf
        memset(RecvBuf, '\0', sizeof(RecvBuf));

        cin.getline(SendBuf, sizeof(SendBuf));                      // Ожидание ввода команды

        if (!_stricmp(SendBuf, PWD)) {                              // Если запросили pwd, то 
            send(ConnectSocket, SendBuf, (int)strlen(SendBuf), 0);  // отправляем команду серверу
            recv(ConnectSocket, RecvBuf, RecvBufLen, 0);            // Ожидаем получения результата и 
            cout << " Корневая папка сервера: " << RecvBuf;         // выводим директорию сервера
        }
        else if (!_stricmp(SendBuf, _DIR)) {                        // Если запросили dir, то
            send(ConnectSocket, SendBuf, (int)strlen(SendBuf), 0);  // отправляем команду серверу

            recv(ConnectSocket, RecvBuf, RecvBufLen, 0);            // Ожидаем данные от сервера
            if (_stricmp(RecvBuf, ERR)) {
                cout << "\tФайлы и папки на сервере: ";
                while (_stricmp(RecvBuf, END_FILE)) {               // Пока не получен флаг окончания
                    printf("\n\t%s", RecvBuf);                      // передачи от сервера - выводим file name

                    send(ConnectSocket, EHO, 1, 0);                 // Даем сигнал что данные из TCP канала прочитаны
                    memset(RecvBuf, '\0', sizeof(RecvBuf));         // и можно отправлять следующую порция байт
                    Result = recv(ConnectSocket, RecvBuf, RecvBufLen, 0);   // Ожидаем данные
                }
            }
            else                                                    // Если вернулась ошибка, то 
                cout << " [ошибка открытия директории сервера]";    // выводим сообщение об этом
        }
        else if (!_strnicmp(SendBuf, PUT, 3)) {                     // Если команда put, то
            char* fName = SendBuf + 4;                              // извлекаем из буфера имя файла  
            if ((f = fopen(fName, "rb")) == NULL)                   // и пробуем открыть его
                cout << " [файл не найден]";                        // Если не получилось, файл не найден
            else {
                send(ConnectSocket, SendBuf, (int)strlen(SendBuf), 0);  // Иначе отправляем данные на сервер
                Result = recv(ConnectSocket, RecvBuf, RecvBufLen, 0);   // и ожидаем ответа 
                if (!_stricmp(SendBuf, ERR))                             // Если получена ошибка, то сервер
                    cout << " [ошибка создания файла на сервере]";      // не смог создать файл
                else if (Result) {                                      // Иначе если сервер не отключился, то
                    while (!feof(f)) {                                  // пока не достигнут конец файла

                        memset(SendBuf, '\0', sizeof(SendBuf));         // готовим буфер для считывания из файла
                        int rb = fread(SendBuf, 1, sizeof(SendBuf), f); // считываем порцию байт из файла

                        if (!rb) break;
                        send(ConnectSocket, SendBuf, rb, 0);            // отправляем на сервр считанное колич байт
                        recv(ConnectSocket, EHO, 1, 0);                 // ожидаем эхо-ответ о приеме данных
                    }
                    send(ConnectSocket, END_FILE, strlen(END_FILE), 0); // отправка сообщения об окончании передачи
                    cout << " [файл передан на сервер]";
                }
                else {
                    cout << "\n [сервер недоступен]";
                    break;
                }
                fclose(f);
            }
        }
        else if (!_stricmp(SendBuf, BYE))       // Если введена комна 'bye' - завершение программы
            break;                              // При этом функция recv() на стороне сервера вернет 0
        else                                    // и серверное приложение так же завершит работу
            cout << " [неизвестная команда]";
    }

    printf("\n [Соединение закрыто]\n");
    closesocket(ConnectSocket);
    WSACleanup();
}

void ListenReguest(SOCKET& ListenSocket) {
    printf("\n %40s", ">> SERVER <<");

    if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("\n Listen failed with error: %d", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        exit(-1);
    }
    printf("\n\n Ожидание подключения клиента..\n");
}

SOCKET CreateClientSock(SOCKET& ListenSocket) {
    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("\n Accept failed with error: %d", WSAGetLastError());
        closesocket(ClientSocket);
        WSACleanup();
        exit(-1);
    }
    printf("\n Подключился клиент, ожидаем получения команд\n");
    return ClientSocket;
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=70 lines=40");

    addrinfo* result = NULL;
    SOCKET Socket, ClientSocket;                //ConnectSocket or ListenSocket;

    InitWinSock();                              // Инициализация WinSock

    result = InitAddrInfo();                    // Инициализация addrinfo для Клиента
    Socket = CreateSocket(result);              // Создаем сокет для Клиента
    if (ConnectToServer(Socket, result)) {      // Пробуем подключиться к серверу

        Client(Socket);
    }
    else {                                      // Если сервер еще не существует, то:
        result = InitAddrInfo(true);            // Инициализация addrinfo для Сервера
        Socket = CreateSocket(result);          // Создаем сокет для Сервера
        BindSockAndAddr(Socket, result);        // Связываем сокет с сетевым адресом
        ListenReguest(Socket);                  // Прослушиваем входящие запросы
        ClientSocket = CreateClientSock(Socket);// Сокет для принятия сообщения от клиента

        Server(ClientSocket);
    }

    cout << endl;  system("pause");
    return 0;
}