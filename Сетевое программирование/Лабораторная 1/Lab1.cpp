#include <windows.h>
#include <conio.h>
#include <iostream>
#include <direct.h>

using namespace std;

#define SYN		0x16		//синхронизирующий символ
#define SOH		0x01		//начало заголовка пакета
#define STX		0x02		//начало текста
#define ETX		0x03		//конец текста
#define ETB		0x17		//конец передачи блока (заголовка)
#define ENQ		0x05		//запрос
#define ACK0		0x06		//четное подтверждение
#define ACK1		0x08		//нечетное подтверждение
#define NAK		0x15		//отрицательная квитанция (переспрос)
#define EOT		0x04		//конец сеанса связи
#define DLE		0x10		//спец. управляющий символ

HANDLE hCOM;
bool dle;
unsigned char f1;
char text[1024];
int text_pos;

void send_char(const char c, bool double_dle = true)
{
	DWORD nb = 0;
	if (c == DLE && double_dle)					// Если при отправке данных встретили символ DLE, то
		WriteFile(hCOM, &c, sizeof(c), &nb, 0);	// удваиваем его, для отделения от управляющих символов.
	WriteFile(hCOM, &c, sizeof(c), &nb, 0);
}
void send_control(const char c)					// Отправка управляющего символа.
{
	send_char(DLE, false);						// Предваряем служебный символ символом DLE.
	send_char(c);
}
void send_text(const char* str)					// Отправка данных.
{
	for (int i = 0; i < strlen(str); i++)
		send_char(str[i]);
}
bool recv()										// Получение данных.
{
	unsigned char c = 0;
	DWORD nb = 0;

	bool res = ReadFile(hCOM, &c, sizeof(c), &nb, NULL);
	int error = GetLastError();
	res = res && nb > 0;

	//Читаем из порта 1 байт
	if (res)
	{
		if (!dle)									// Если предыдущий символ не DLE:
		{
			f1 = 0;
			if (c == DLE)							// Если получен символ DLE, запоминаем это,
				dle = true;
			else										// иначе заполняем буфер данными	.
				text[text_pos++] = c;				
		}
		//Ожидается управляющий символ
		else											// Если предыдущий был DLE:
		{
			//Обработка управляющих символов
			if (c == DLE)							// Если получен второй сподряд символ DLE 
			{										// то рассматриваем его как данные.
				//Обработать DLE как обычный символ
				text[text_pos++] = c;
			}
			else										// Иначе получен управляющий символ.
			{
				f1 = c;
				if (c == SOH || c == STX)			// Если получен символ начала передачи 
				{									// блока заголовка или данных, то:
					text_pos = 0;					// установка позиции буфера в начало;
					memset(text, 0, sizeof(text));	// очистка буфера (подготовка к приему данных).
				}
			}
			dle = false;
		}
	}

	return res;
}

void close()
{	
	CloseHandle(hCOM);			// закрываем описатель
	ExitProcess(0);				// завершаем программу
}

void open_port(wchar_t *PORT)
{
	hCOM = CreateFile(  PORT,								// имя порта
						GENERIC_READ | GENERIC_WRITE,		// доступ для чтения, записи
						0,
						NULL,
						OPEN_EXISTING,						// атрибут защиты
						FILE_ATTRIBUTE_NORMAL,				// атрибут создаваемого файла
						NULL	
					);
	if (hCOM == INVALID_HANDLE_VALUE)						// если получили описатель, продолжаем
	{
		cout << " Ошибка открытия порта" << endl;			// иначе завершаем программу
		close();					
	}
										  // настраиваем порт
	DCB dcb;								  // Структура для хранения конфигурации порта
	GetCommState(PORT, &dcb);			  // Извлекаем настройки управления
	dcb.fBinary = TRUE;                   // Включаем двоичный режим обмена
	dcb.BaudRate = CBR_9600;              // Скорость передачи данных
	dcb.ByteSize = 8;                     // Размер байта данных
	dcb.StopBits = ONESTOPBIT;            // Количество стоповых бит
	dcb.fParity = NOPARITY;               // Проверка четности

	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = 1000;

	if (!SetCommState(hCOM, &dcb) || !SetCommTimeouts(hCOM, &CommTimeOuts))
	{
		cout << " Ошибка настройки порта" << endl;
		close();
	}
}

void reset()
{
	dle = false;
	f1 = 0;
	text_pos = 0;
	memset(text, 0, sizeof(text));
	cout << "\n S - начать передачу, ESC - выход\n Ожидаем...\n";
}

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_ALL, ".1251");
	system("cls");
	
	if (argc != 2)
	{
		cout << " Не задано имя порта" << endl;
		return 0;
	}

	wchar_t portname[MAX_PATH] = L"\\\\.\\";
	wcscat_s(portname, argv[1]);

	wcout << L"\n Открываем порт: " << argv[1] << endl;
	open_port(portname);

	char *headerbuff = _getcwd(NULL, 0);		// получаем имя директории в переменную buffer

	const char* databuff = "Tryushnikov Vadim Georgievich";
		
	reset();
	do
	{
		if (_kbhit())
		{
			char c = _getch();
			if ((int)c == 115 || (int)c == 83) {
				send_control(ENQ);							//	запрос на установку связи
				cout << " Запрос на соединение.." << endl;
			}

			if ((int)c == 27)
				close();
		}
		else
			if (recv())
			{
				switch (f1)
				{
				case ENQ:
					send_control(ACK0);
					break;
				case ACK0:
					cout << " Соединение успешно\n Передаем данные" << endl;
					send_control(SOH);
					send_text(headerbuff);	// передаем блок заголовка
					send_control(ETB);
					break;
				case ETB:
					cout << "\n Получили данные: ";
					cout << text;
					send_control(ACK1);
					break;
				case ACK1:
					send_control(STX);
					send_text(databuff);		// передаем пакет данных
					send_control(ETX);
					break;
				case ETX:
					cout << text << endl;
					send_control(EOT);		// разрыв соединения
					reset();
					break;
				case EOT:
					cout << " Данные переданы\n Разрыв соединения" << endl;
					reset();
					break;
				}
			}
	} while (true);
	
	return 0;
}
