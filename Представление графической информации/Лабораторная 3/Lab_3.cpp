#include <fstream>
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

enum Mode{ bmp16, bmp256, bmpTC } BMP;

unsigned char palitra256[256][4];
unsigned char palitra16 [16] [4];

void Draw(ifstream& f, Mode BMP, unsigned char table[][4]=0){
	
	char Color; 
	int R, G, B, c;
	
	for(int Y=head_file.biheight-1; Y >= 0; Y--){		// движемся по Y снизу вверх
		for(int X=0; X < head_file.biwidth; X++){		// русуем по X слева направо
			
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
							 
				case bmpTC: f.get(Color); R = (int)(unsigned char)Color;
							f.get(Color); G = (int)(unsigned char)Color;
							f.get(Color); B = (int)(unsigned char)Color;
							break;
			}
			
			putpixel(X,Y,RGB(R,G,B));
		}
		if(head_file.biwidth % 4) 					// пропускаем лишнее (если есть)
			f.seekg(4 - head_file.biwidth % 4, ios::cur);
	}
}

void main() {
	
	//----------- TEST FILES:
	const char* FileName = "_Carib256.bmp";
	//const char* FileName = "_CaribTC.bmp";		
	//const char* FileName = "_Carib16.bmp";
	//const char* FileName = "CAT16.bmp";
	//const char* FileName = "CAT256.bmp";
	//const char* FileName = "logoTC.bmp";

	ifstream f1(FileName, ios::binary);
	
	if(!f1) { exit(-1); }
	
	f1.read((char*)&head_file, sizeof(head));			// читаем заголовок файла
	
	resize(head_file.biwidth, head_file.biheight);		// создаем графическое окно
	
	switch (head_file.bibitcount){
		
		case 4 : f1.read((char*)&palitra16, sizeof(palitra16));
				 Draw(f1, bmp16, palitra16);
				 break;
				 
		case 8 : f1.read((char*)&palitra256, sizeof(palitra256));
				 Draw(f1, bmp256, palitra256);
				 break;
		
		case 24: Draw(f1, bmpTC);
				 break;
				 
		default: exit(-1);
	}
  
  return;
}