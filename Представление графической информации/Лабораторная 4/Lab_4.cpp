#include <iostream>
#include <windows.h>
#include <fstream>
#include <cstdio>
#include <cstring>

using namespace std;

#pragma pack(push, 1)
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

} hFile, hLogo;
#pragma pack(pop)

void DrawLogo(fstream& file, ifstream& logo){
	
	// смещаем указатель картинки на размер
	// логотипа относительно конца данных:
	file.seekp(-(hFile.biwidth * hLogo.biheight * 3), ios::end);
	
	char Data;
	for(int Y=0; Y < hLogo.biheight; Y++){
		for(int X=0; X < hLogo.biwidth; X++){
			logo.get(Data); file.put(Data);
			logo.get(Data); file.put(Data);
			logo.get(Data); file.put(Data);
		}
		file.seekp((hFile.biwidth - hLogo.biwidth) * 3, ios::cur);
		
		// проверка выравнивания строки по 32 битной границе или
		// 4 байта и при необходимости пропуск лишних байт:
		if(hLogo.biwidth % 4)
			logo.seekg(4 - hLogo.biwidth % 4, ios::cur);
	}
}

int main() {
	
	SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
	
	char FileName[100] = "_CaribTC.bmp";
	const char* Logotip  = "logoTC.bmp";
	
	ifstream Image(FileName, ios::binary);
	ifstream Logo(Logotip, ios::binary);
	
	if(!Image || !Logo) { 
		cout << "\n No file " << FileName << " or " << Logotip << endl << endl;
		system("pause");
		return -1; 
	}
	
	memset(FileName + strlen(FileName)-4, '\0', 4);
	
	fstream NewFile(strcat(FileName, "logo.bmp"), ios::in	| ios:: out 
												| ios::trunc| ios::binary);
	
	NewFile << Image.rdbuf(); Image.close();		// копируем файл и закрываем orig
	
	NewFile.seekg(ios::beg);
	
	NewFile.read((char*)&hFile, sizeof(hFile));		// читаем заголовок файла
	Logo.read	((char*)&hLogo, sizeof(hLogo));		// читаем заголовок логотипа
	
	DrawLogo(NewFile, Logo);
	
	Logo.close();
	NewFile.close();
	
	cout << "\n * Новый файл сохранен как: " << FileName << endl << endl;
	
	system("pause");
  
  return 0;
}