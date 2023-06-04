#include <fstream>
#define MAXCOLORTC
#include "wingraph.h"

using namespace std;

#pragma pack(push, 1)
typedef struct TPCXHeaderStruct{
  unsigned char ID;
  unsigned char Version;
  unsigned char Coding;
  unsigned char BitPerPixel;
  unsigned short  XMin;
  unsigned short  YMin;
  unsigned short  XMax;
  unsigned short  YMax;
  unsigned short  HRes;
  unsigned short  VRes;
  unsigned char Palette[48];
  unsigned char Reserved;
  unsigned char Planes;
  unsigned short  BytePerLine;
  unsigned short  PaletteInfo;
  unsigned short  HScreenSize;
  unsigned short  VScreenSize;
  unsigned char Filler[54];
} TPCXHeader;

typedef struct TPaletteStruct{
  unsigned char Red;
  unsigned char Green;
  unsigned char Blue;
} TPalette;
 #pragma pack(pop)

TPCXHeader PCXHeader;
TPalette PCXPalette[256];

void DrawImage(ifstream& f, int TotalBytes, int Width, int Height){
	
	unsigned char Dbyte;
	int R, G, B, c = 0;
	resize(Width, Height);					// изменяем размер холста
	f.seekg(128, ios::beg);					// указатель на начало данных
	
	for(int Y=0; Y < Height; Y++){
		
		int X = 0;
		while(X < TotalBytes){
		
			f.get(*(char*)&Dbyte);			// читаем байт данных
			
			if((int)Dbyte >> 6 == 0b11){	// если 2 старших бита = 11
				c = (int)Dbyte & 0x3F;		// получаем колич повторов 
				f.get(*(char*)&Dbyte);		// следующего байта данных
				while(c--){					// и выводим нужное число px
							
					putpixel(X, Y, RGB((int)PCXPalette[Dbyte].Red,
									   (int)PCXPalette[Dbyte].Green,
									   (int)PCXPalette[Dbyte].Blue));
					X++;
				}
			}
			else{							// иначе байт = <данные>
				
				putpixel(X, Y, RGB((int)PCXPalette[Dbyte].Red,
								   (int)PCXPalette[Dbyte].Green,
								   (int)PCXPalette[Dbyte].Blue));
									   
				X++;
			}
		}
	}
}

void main(){
	
	const char* FileName = "CAT256.PCX";
	
	ifstream fs(FileName, ios::binary);
	
	if(!fs){ exit(-1); }
	
	fs.read((char*)&PCXHeader, sizeof(PCXHeader));
	
	if((int)PCXHeader.Version != 5 || (int)PCXHeader.BitPerPixel != 8){
		fs.close();
		exit(-1);
	}
	
	fs.seekg(-sizeof(PCXPalette), ios::end);		// смещаем указатель на палитру
	fs.read((char*)&PCXPalette, sizeof(PCXPalette));// читаем палитру
	
	int X = PCXHeader.XMax - PCXHeader.XMin + 1,	// определяем размер изображения
		Y = PCXHeader.YMax - PCXHeader.YMin + 1,	// и
		TotalBytes = (int)PCXHeader.Planes * 		// размер одной не сжатой строки!
						  PCXHeader.BytePerLine;
			
	DrawImage(fs, TotalBytes, X, Y);
	
	fs.close();
}