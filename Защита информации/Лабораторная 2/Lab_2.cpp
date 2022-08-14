#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <iomanip>
#include <cmath>
#include <fstream>

using namespace std;

unsigned int K[8]{};								// ���� ����������
unsigned int Y[2]{};								// ���� ����������� ��� ���������
unsigned int W[32]{};							// ��������� ����
unsigned int S[8][16]{};							// S - �����
unsigned int Y0[2]{ 0xACDC, 0xCDCA };			// ��������� ������

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
		cout << "\n\t\t\t>> ������������ ������ �2 <<\n\n";
		cout << "\t�������� ��������: 1 - ���������, 2 - �����������: ";
		Choice = _getche();
	} while (Choice != '1' && Choice != '2');

	switch (Choice) 
	{
		case '1': {
			do {
				cout << "\n\n\t�������� ����� ����������:";
				cout << "\n\t1 - ECB, 2 - CBC, 3 - OFB, 4 - CTR: ";
				Choice = _getche();
				if (Choice >= '1' && Choice <= '4') break;
				cout << "\n\t[ERROR INPUT]";
			} while (true);

			cout << "\n\n\t������� ������ ��� ����������:\t"; 
			getline(cin, UserMsg);
			cout << "\t������� ���� ����������:\t"; 
			getline(cin, UserKey);

			CreateKeys(UserKey, 77);

			ofstream out(GetFileName(Choice), ios::out | ios::binary);
			if (!out.is_open()) {
				cout << "\n\t[���������� ������� ����: "+ GetFileName(Choice)+"]";
				system("pause"); 
				exit(-1);
			}

			for (int i = 0; i < UserMsg.length(); i += 8) {

				unsigned char X[8]{};
				memset(X, 32, 8);
				int CountByte = strlen(UserMsg.c_str() + i) < 8 ? strlen(UserMsg.c_str() + i) : 8;
				memcpy(X, UserMsg.c_str() + i, CountByte);
				long long temp = *(long long*)X;

				if (Choice == '2') *(long long*)X ^= *(long long*)Y0;	// ����� CBC
				if (Choice  > '2') *(long long*)X =  *(long long*)Y0;	// ����� OFB, CTR

				Shifr(X);

				if (Choice == '2') *(long long*)Y0 = *(long long*)Y;		// ����� CBC
				if (Choice == '3') {										// ����� OFB
					*(long long*)Y0 = *(long long*)Y;
					*(long long*)Y ^= temp;
				}
				if (Choice == '4') {										// ����� CTR
					*(long long*)Y0 += 1;
					*(long long*)Y ^= temp;
				}

				//memcpy(EncryptMsg + i, Y, 8);

				out.write((char*)Y, sizeof(Y));

			}
			out.close();
			cout << "\n\t[��������� � ����: " << GetFileName(Choice) << "]\n\n";

			//cout << EncryptMsg << endl;
		}
		break;

		case '2': {
			do {
				cout << "\n\n\t�������� ����� ������������:";
				cout << "\n\t1 - ECB, 2 - CBC, 3 - OFB, 4 - CTR: ";
				Choice = _getche();
				if (Choice >= '1' && Choice <= '4') break;
				cout << "\n\t[ERROR INPUT]";
			} while (true);

			cout << "\n\n\t������� ���� ������������:\t";
			getline(cin, UserKey);

			CreateKeys(UserKey, 77);
			if(Choice < '3') reverse(begin(W), end(W));					// ����� OFB, CTR

			ifstream in(GetFileName(Choice), ios::in | ios::binary);
			if (!in.is_open()) {
				cout << "\n\t[�� ������ ���� � ������������]\n" << endl;
				system("pause"); 
				exit(-1);
			}

			ofstream out("DecryptMessage.txt", ios::out | ios::binary);
			if (!out.is_open()) {
				cout << "\n\t[���������� ������� ����: DecryptMessage.txt]";
				system("pause"); 
				exit(-1);
			}

			int i = 0;
			while (true) {
				unsigned char X[8]{};
				in.read((char*)X, sizeof(X));
				long long temp = *(long long*)X;

				if (in.eof()) break;

				if (Choice > '2') *(long long*)X = *(long long*)Y0;		// ����� OFB, CTR

				Shifr(X);

				if (Choice == '2') {										// ����� CBC
					*(long long*)Y ^= *(long long*)Y0;
					*(long long*)Y0 = temp;
				}
				if (Choice == '3') {										// ����� OFB
					*(long long*)Y0 = *(long long*)Y;
					*(long long*)Y ^= temp;
				}
				if (Choice == '4') {										// ����� CTR
					*(long long*)Y0 += 1;
					*(long long*)Y ^= temp;
				}

				//memcpy(DecryptMsg + i++ * 8, Y, 8);

				out.write((char*)Y, sizeof(Y));
			}

			in.close();
			out.close();
			cout << "\n\t[��������� � ����: DecryptMessage.txt]\n\n";

			//cout << DecryptMsg << endl;
		}
		break;
	}

	system("pause");

	return 0;
}
