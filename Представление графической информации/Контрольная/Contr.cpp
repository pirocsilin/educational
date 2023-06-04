#include <iostream>
#include <fstream>
#include <vector>
#define MAXCOLORTC
#include "wingraph.h"

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
} head_file;
#pragma pack(pop)

enum Mode{ bmp16, bmp256} BMP;

int GetColor(unsigned char);
void Converter(ifstream&, ofstream&);
void Draw(ifstream&, Mode, unsigned char[][4], int, int);

unsigned char palitra[256][4]{};

// Палитра по умолчанию имеет средние значения цветов исходного классификатора
unsigned char palitra16[16][4] = {{64,32,64,0},  {64,96,64,0},  {64,160,64,0},  {64,224,64,0}, 
								  {64,32,192,0}, {64,96,192,0}, {64,160,192,0}, {64,224,192,0},
								  {192,32,64,0}, {192,96,64,0}, {192,160,64,0}, {192,224,64,0},
								  {192,32,192,0},{192,96,192,0},{192,160,192,0},{192,224,192,0}};
struct Color {
	int R, G, B, Class;
	
	Color(int r=0, int g=0, int b=0, int c=0) : R(r), G(g), B(b), Class(c){}
	
	int Delta (Color& op2){
		return pow(R - op2.R, 2) + pow(G - op2.G, 2) + pow(B - op2.B, 2);
	}
	
	// Классификатор для 16-ти цветной палитры
	void SetClass(){
		R <= 128 && G <= 64 && B <= 128 			? Class = 0:
		R <= 128 && G > 64 && G <= 128 && B <= 128  ? Class = 1:
		R <= 128 && G > 128 && G <= 192 && B <= 128 ? Class = 2:
		R <= 128 && G > 192 && B <= 128				? Class = 3:
		
		R <= 128 && G <= 64 && B > 128 			   ? Class = 4 :
		R <= 128 && G > 64 && G <= 128 && B > 128  ? Class = 5 :
		R <= 128 && G > 128 && G <= 192 && B > 128 ? Class = 6 :
		R <= 128 && G > 192 && B > 128 			   ? Class = 7 :
		
		R > 128 && G <= 64 && B <= 128 			   ? Class = 8 :
		R > 128 && G > 64 && G <= 128 && B <= 128  ? Class = 9 :
		R > 128 && G > 128 && G <= 192 && B <= 128 ? Class = 10:
		R > 128 && G > 192 && B <= 128 			   ? Class = 11:
		
		R > 128 && G <= 64 && B > 128 			  ? Class = 12 :
		R > 128 && G > 64 && G <= 128 && B > 128  ? Class = 13 :
		R > 128 && G > 128 && G <= 192 && B > 128 ? Class = 14 :
													Class = 15 ;
	}
};
vector<Color> arr(256);

void main() {
	
	const char* FileName = "CAT256.BMP";
	const char* NewFile  = "CAT_from_256_to_16.BMP";

	ifstream f1(FileName, ios::binary);
	
	if(!f1) {
		cout << "\n Файл: " << FileName << " не найден!\n"; 
		exit(-1);
	}
	
	f1.read((char*)&head_file, sizeof(head));
	f1.read((char*)&palitra, sizeof(palitra));
	
	resize(head_file.biwidth * 2 + 80, head_file.biheight + 80);	// создаем холст
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 20);
	outtextxy(20, 20, "Исходное изображение, 256 цветов:", LIGHTGRAY);
	outtextxy(head_file.biwidth + 50, 20, "Конвертированное изображение, 16 цветов:", LIGHTGRAY);
	
	Draw(f1, bmp256, palitra, 20, 50);				// выводим исходный рисунок
	
	for(int i=0; i<256; i++){
		arr[i] = Color((int)palitra[i][0],
					   (int)palitra[i][1],
					   (int)palitra[i][2]);
					   
		arr[i].SetClass();							// установка цветового класса
	}
	
	for(int j=0; j<16; j++){
		int R = 0, G = 0, B = 0, Count = 0;
		for(int i=0; i<256; i++){
			if(arr[i].Class == j){
				R += arr[i].R;
				G += arr[i].G;
				B += arr[i].B;
				Count++;
			}
			if(Count){								// если есть точки попавшие в класс
				palitra16[j][0] = R / Count;		// то усредняем цвета всех компонент
				palitra16[j][1] = G / Count;		// и записываем новый элемент
				palitra16[j][2] = B / Count;		// 16-и цветной палитры, иначе
			}										// цвет останется по умолчанию
		}
	}
	
	ofstream f2(NewFile, ios::binary);
													// установка служебной информации
	head_file.bfsize =  (4 - head_file.biwidth % 4 + 
						head_file.biwidth) * head_file.biheight / 2 + 
						sizeof(palitra16) + sizeof(head_file);
						
	head_file.bisizeimage = head_file.biwidth * head_file.biheight / 2;
	head_file.bfoffbits = sizeof(palitra16) + sizeof(head_file);
	head_file.bibitcount = 4;
	head_file.biclrused = 16;
	head_file.biclrimp = 16;
	
	f2.write((char*)&head_file, sizeof(head_file));
	f2.write((char*)&palitra16, sizeof(palitra16));
	
	Converter(f1, f2);

	f1.close();
	f2.close();
													// выводим новый рисунок
	ifstream _f2(NewFile, ios::binary);
	Draw(_f2, bmp16, palitra16, head_file.biwidth + 50, 50);
	_f2.close();
}

void Draw(ifstream& f, Mode BMP, unsigned char table[][4], int dX, int dY){
	
	char Color; 
	int R, G, B, c;
	
	f.seekg(head_file.bfoffbits, ios::beg);				// указатель на начало образа
	for(int Y=head_file.biheight-1; Y >= 0; Y--){
		for(int X=0; X < head_file.biwidth; X++){
			
			switch (BMP){
				case bmp16: if(X%2==0){
								f.get(Color);
								c = (int)(unsigned char)Color >> 4;
							}
							else
								c = (int)(unsigned char)Color & 0xF;
							
							R = table[c][0]; 
								G = table[c][1]; 
									B = table[c][2];
							break;
							
				case bmp256: 
							 f.get(Color);
							 c = (int)(unsigned char)Color;
							 R = table[c][0]; 
								G = table[c][1]; 
									B = table[c][2];
							 break;
			}
			putpixel(X + dX, Y + dY, RGB(R,G,B));
		}
		if(head_file.biwidth % 4 && BMP == bmp256)
			f.seekg(4 - head_file.biwidth % 4, ios::cur);
	}
	f.seekg(head_file.bfoffbits, ios::beg);
}

void Converter(ifstream& f1, ofstream& f2){
	
	unsigned char ch, Data;
	
	for(int Y=0; Y < head_file.biheight; Y++){
		for(int X=0; X < head_file.biwidth; X++){
			
			f1.get(*(char*)&ch);					// получаем ромер 8-и битного цвета
			int Color = GetColor(ch);				// получаем номер 4-х битного цвета
			
			if(X % 2 == 0){							// формируем цвет в старших
				Data = *(unsigned char*)&Color;		// 4-х битах одного байта
				Data <<= 4;
			}
			else{
				Data |= *(unsigned char*)&Color;	// формируем цвет в младших
				f2.put(*(char*)&Data);				// 4-х битах одного байта
			}
		}
		if(head_file.biwidth % 4){
			char EmptyPlase[] = {0,0,0,0};
			f1.seekg(4 - head_file.biwidth % 4, ios::cur);	 // пропускаем лишнее
			f2.write(EmptyPlase, 4 - head_file.biwidth % 4); // выравниваем до 4 байт
		}
	}
}

int GetColor(unsigned char n){			
	
	Color _c256 = { palitra[n][0], 
					palitra[n][1], 
					palitra[n][2] };
					
	int MinDelta = INT_MAX, c = 0;
	
	for(int i=0; i<16; i++){
		Color _c16 = { palitra16[i][0], 
					   palitra16[i][1], 
					   palitra16[i][2] };
					   
		if( _c256.Delta(_c16) < MinDelta){
			MinDelta = _c256.Delta(_c16);
			c = i;
		}
	}
	return c;
}