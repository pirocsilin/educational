#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstring>

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
} head_file;
#pragma pack(pop)	// включаем выравнивание

const char* FileOrig = "_Carib256.bmp";
const char* FileCopy = "_Carib256_frame.bmp";
const int SizeFrame = 15;

int main() {
	
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

	ifstream f1(FileOrig, ios::binary);				// открываем orig файл для чтения
	
	if(!f1) {
		cout << "\n Файл: " << FileOrig << " не найден!\n"; 
		return -1;
	}
	
	ofstream f2(FileCopy, ios::binary);				// открываем copy файл для записи
	
	f1.read((char*)&head_file, sizeof(head));		// читаем заголовок файла
	f1.seekg(0, ios::beg);							// возврат указателя на начало файла
	f2 << f1.rdbuf();								// копируем файл orig -> copy
	
	f1.close();										// закрываем orig файл
	
	f2.seekp(head_file.bfoffbits, ios::beg);		// смещаем указатель на битовый образ
	
	int Bytes = 0;
	while(Bytes < head_file.bisizeimage){			// создаем рамку вокруг изображения
		
		if(Bytes < head_file.biwidth * SizeFrame || 
		   Bytes >= head_file.bisizeimage - head_file.biwidth * SizeFrame){
			f2.put(rand() % 256);
			Bytes++;
		}
		else{
			for(int i=0; i<SizeFrame; i++){
				f2.put(rand() % 256);
				Bytes++;
			}
			
			f2.seekp(head_file.biwidth - SizeFrame * 2, ios::cur);
			Bytes += head_file.biwidth - SizeFrame * 2;
			
			for(int i=0; i<SizeFrame; i++){
				f2.put(rand() % 256);
				Bytes++;
			}
		}
	}
	
	f2.close();
	
	cout << "\n * Новый файл сохранен как: " << FileCopy << endl << endl;
	
	system("pause");
  
  return 0;
}