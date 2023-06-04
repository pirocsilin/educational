#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstring>
#include <iomanip>

using namespace std;

#pragma pack(push, 1)	// отключаем выравнивание данных в структуре
struct  head {
    short bftype;
    long  bfsize;
    short rez1;
	short rez2;
    long  bfoffbits;
    long  bisize;
    long  biwidth;
    long  biheight;
    short biplanes;
    short bibitcount;
    long  bicompression;
    long  bisizeimage;
    long  bix;
    long  biy;
    long  biclrused;
    long  biclrimp;
	
	void PrintInfo(char* fNmae){
		cout << endl << setw(33) << "Параметры файла: " << fNmae
			 << "\n тип файла:\t\t\t\t\t\t" << *(char*)&bftype << *((char*)&bftype+1) 
			 << "\n размер файла:\t\t\t\t\t\t" << bfsize
			 << "\n смещение данных битового образа от заголовка в байтах:\t" << bfoffbits
			 << "\n число байт, занимаемых структурой InfoHeader:\t\t" << bisize
			 << "\n ширина битового образа в пикселях:\t\t\t" << biwidth
			 << "\n высота битового образа в пикселях:\t\t\t" << biheight
			 << "\n число битовых плоскостей устройства:\t\t\t" << biplanes
			 << "\n число битов на пиксель:\t\t\t\t" << bibitcount
			 << "\n число используемых цветов:\t\t\t\t" << biclrused << endl;
	}
} head_file;
#pragma pack(pop)	// включаем выравнивание

unsigned char palitra[256][4];

int main(int argc, char *argv[]) {
	
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
	
	char FileName[100];
	
	if(argc == 1) {
		cout << "\n Не указано имя файла!\n"; 
		return -1;
	}
	
	if(strcmp(".bmp", argv[1] + strlen(argv[1])-4)){
		cout << "\n Файл должен иметь расширение .bmp!\n"; 
		return -1;
	};

	ifstream f1(strcpy(FileName, argv[1]), ios::binary);
	
	if(!f1) {
		cout << "\n Файл: " << argv[1] << " не найден!\n"; 
		return -1;
	}
	
	memset(FileName + strlen(argv[1])-4, '\0', 4);	// убираем расширение файла
	
	fstream f2(strcat(FileName, "_copy.bmp"), ios::in | ios::out | ios::binary | ios::trunc);
	
	f1.read((char*)&head_file, sizeof(head));		// читаем заголовок файла
	
	if(head_file.bibitcount != 8){
		cout << "\n Файл не имеет 256 цветную палиту!\n"; 
		return -1;
	}
	
	f1.seekg(0, ios::beg);							// возврат указателя на начало файла
	f2 << f1.rdbuf();								// копируем файл для модификации
	
	f1.close();										// закрываем оригинальный файл и
	head_file.PrintInfo(argv[1]);					// выводим основную инфу о нем
	
	f2.seekg(54, ios::beg);							// смещаем указатель копии на
	f2.read((char*)&palitra, sizeof(palitra));		// начало палитры и читаем её
	
	for(int i=0; i<256; i++){						// усредняем цвета в палитре
		int avrg = 0;
		for(int j=0; j<3; j++){
			avrg += (int)palitra[i][j];
		}
		for(int j=0; j<3; j++){
			palitra[i][j] = avrg / 3;
		}
	}
	
	f2.seekp(54, ios::beg);							// смещаем указатель на начало таблицы
	f2.write((char*)&palitra, sizeof(palitra));		// записываем новую палитру в новый файл
	
	f2.close();
	
	cout << "\n * Новый файл сохранен как: " << FileName << endl;
  
  return 0;
}