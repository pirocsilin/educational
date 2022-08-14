#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

unsigned int K[8]{};					// ключ шифрования
unsigned int Y[2]{};					// блок шифрограммы или сообщения
unsigned int W[32]{};					// раундовый ключ
unsigned int S[8][16]{};				// S - боксы
unsigned int Y0[2]{ 0xACDC, 0xCDCA };			// начальный вектор

void Shifr(unsigned char X[]) {

	unsigned int L{}, R{};
	memcpy(&L, X, 4);
	memcpy(&R, X + 4, 4);

	for (int v = 0; v < 32; v++) {

		unsigned int k = R + W[v], KeyEncrypt = 0;
		for (int j = 0; j < 8; j++) {
			
			KeyEncrypt = KeyEncrypt | S[j][k & 0b1111];
			KeyEncrypt <<= 4;
			k >>= 4;
		}
		KeyEncrypt = (KeyEncrypt << 11) | (KeyEncrypt >> (32 - 11));
		L ^= KeyEncrypt;
		if(v < 31) swap(L, R);
	}
	memcpy(Y, &L, 4);
	memcpy(Y + 1, &R, 4);
}

void CreateKeys(string UserKey, int Init_S_box) {

	srand(Init_S_box);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			S[i][j] = rand() % 16;
		}
	}

	unsigned int CountByte = UserKey.length() < 32 ? UserKey.length() : 32;
	memcpy(K, UserKey.c_str(), CountByte);

	for (int i = 0; i < 32; i += 8) {
		if (i == 24)
			reverse(begin(K), end(K));
		memcpy(W + i, K, 32);
	}
}

string GetFileName(char Choice) {
	return	Choice == '1' ? "EncryptMessageECB.txt" :
			Choice == '2' ? "EncryptMessageCBC.txt" :
			Choice == '3' ? "EncryptMessageOFB.txt" :
							"EncryptMessageCTR.txt" ;
}

unsigned int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=90 lines=25");

	string UserMsg, UserKey;
	//char EncryptMsg[50]{};
	//char DecryptMsg[50]{};
	char Choice{};

	do {
		system("cls");
		cout << "\n\t\t\t>> Лабораторная работа №2 <<\n\n";
		cout << "\tВыберите действие: 1 - Шифровать, 2 - Дешифровать: ";
		Choice = _getche();
	} while (Choice != '1' && Choice != '2');

	switch (Choice) 
	{
		case '1': {
			do {
				cout << "\n\n\tВыберите режим шифрования:";
				cout << "\n\t1 - ECB, 2 - CBC, 3 - OFB, 4 - CTR: ";
				Choice = _getche();
				if (Choice >= '1' && Choice <= '4') break;
				cout << "\n\t[ERROR INPUT]";
			} while (true);

			cout << "\n\n\tВведите строку для шифрования:\t"; 
			getline(cin, UserMsg);
			cout << "\tВведите ключ шифрования:\t"; 
			getline(cin, UserKey);

			CreateKeys(UserKey, 77);

			ofstream out(GetFileName(Choice), ios::out | ios::binary);
			if (!out.is_open()) {
				cout << "\n\t[Невозможно открыть файл: "+ GetFileName(Choice)+"]";
				system("pause"); 
				exit(-1);
			}

			for (int i = 0; i < UserMsg.length(); i += 8) {

				unsigned char X[8]{};
				memset(X, 32, 8);
				int CountByte = strlen(UserMsg.c_str() + i) < 8 ? strlen(UserMsg.c_str() + i) : 8;
				memcpy(X, UserMsg.c_str() + i, CountByte);
				long long temp = *(long long*)X;

				if (Choice == '2') *(long long*)X ^= *(long long*)Y0;	// режим CBC
				if (Choice  > '2') *(long long*)X =  *(long long*)Y0;	// режим OFB, CTR

				Shifr(X);

				if (Choice == '2') *(long long*)Y0 = *(long long*)Y;	// режим CBC
				if (Choice == '3') {									// режим OFB
					*(long long*)Y0 = *(long long*)Y;
					*(long long*)Y ^= temp;
				}
				if (Choice == '4') {									// режим CTR
					*(long long*)Y0 += 1;
					*(long long*)Y ^= temp;
				}

				//memcpy(EncryptMsg + i, Y, 8);

				out.write((char*)Y, sizeof(Y));

			}
			out.close();
			cout << "\n\t[Сохранено в файл: " << GetFileName(Choice) << "]\n\n";

			//cout << EncryptMsg << endl;
		}
		break;

		case '2': {
			do {
				cout << "\n\n\tВыберите режим дешифрования:";
				cout << "\n\t1 - ECB, 2 - CBC, 3 - OFB, 4 - CTR: ";
				Choice = _getche();
				if (Choice >= '1' && Choice <= '4') break;
				cout << "\n\t[ERROR INPUT]";
			} while (true);

			cout << "\n\n\tВведите ключ дешифрования:\t";
			getline(cin, UserKey);

			CreateKeys(UserKey, 77);
			if(Choice < '3') reverse(begin(W), end(W));					// режим OFB, CTR

			ifstream in(GetFileName(Choice), ios::in | ios::binary);
			if (!in.is_open()) {
				cout << "\n\t[Не найден файл с шифрограммой]\n" << endl;
				system("pause"); 
				exit(-1);
			}

			ofstream out("DecryptMessage.txt", ios::out | ios::binary);
			if (!out.is_open()) {
				cout << "\n\t[Невозможно открыть файл: DecryptMessage.txt]";
				system("pause"); 
				exit(-1);
			}

			int i = 0;
			while (true) {
				unsigned char X[8]{};
				in.read((char*)X, sizeof(X));
				long long temp = *(long long*)X;

				if (in.eof()) break;

				if (Choice > '2') *(long long*)X = *(long long*)Y0;			// режим OFB, CTR

				Shifr(X);

				if (Choice == '2') {							// режим CBC
					*(long long*)Y ^= *(long long*)Y0;
					*(long long*)Y0 = temp;
				}
				if (Choice == '3') {							// режим OFB
					*(long long*)Y0 = *(long long*)Y;
					*(long long*)Y ^= temp;
				}
				if (Choice == '4') {							// режим CTR
					*(long long*)Y0 += 1;
					*(long long*)Y ^= temp;
				}

				//memcpy(DecryptMsg + i++ * 8, Y, 8);

				out.write((char*)Y, sizeof(Y));
			}

			in.close();
			out.close();
			cout << "\n\t[Сохранено в файл: DecryptMessage.txt]\n\n";

			//cout << DecryptMsg << endl;
		}
		break;
	}

	system("pause");

	return 0;
}
