
// -------------------------- Цифровая подпись на базе RSA ------------------------

#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <string>
#include <fstream>

using namespace std;

unsigned int Y[2]{};								// блок шифрограммы или сообщения
unsigned int H[2]{};								// Хэш - сумма сообщения
unsigned int S[8][16]{};							// S - боксы
int FirstInitSbox = 77;							// Первичная инициализация rand();

const string FileName = "Message.txt";

int FastPower(int a, int x, int p) {

	int LenBin = log2(x) + 1, y = 1, s = a;

	while (LenBin--) {
		if (x & 1) y = y * s % p;
		s = s * s % p;
		x >>= 1;
	}
	return y;
}

int InversionByModule(int c, int p) {

	int U[] = { p, 0 }, V[] = { c, 1 };
	while (V[0] != 0) {
		int q = U[0] / V[0];
		int T[] = { U[0] % V[0], U[1] - q * V[1] };
		swap(U, V);
		swap(V, T);
	}
	return U[1] < 0 ? U[1] + p : U[1];
}

const int q = 787, 
		  p = 31481, 
		  a = 1928, 
		  x = q / 2,								// secret user key	- (0 < x < q)
		  y = FastPower(a, x, p);				// open user key		- (y = а^x mod p)

void Shifr(unsigned char X[]) {

	unsigned int L{}, R{};
	memcpy(&L, X, 4);
	memcpy(&R, X + 4, 4);

	for (int v = 0; v < 32; v++) {

		unsigned int k = R + H[v % 2],			// Ключ шифрования = предыдущей хэш-сумме
					 KeyEncrypt = 0;
		for (int j = 0; j < 8; j++) {

			KeyEncrypt = KeyEncrypt | S[j][k & 0b1111];
			KeyEncrypt <<= 4;
			k >>= 4;
		}
		KeyEncrypt = (KeyEncrypt << 11) | (KeyEncrypt >> (32 - 11));
		L ^= KeyEncrypt;
		if (v < 31) swap(L, R);
	}
	memcpy(Y, &L, 4);
	memcpy(Y + 1, &R, 4);
}

void Init_S_box(int n) {

	srand(n);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 16; j++) {
			S[i][j] = rand() % 16;
		}
	}
}

void GetHashSumm(int FirstInitSbox) {
	ifstream in(FileName, ios::in | ios::binary);
	if (!in.is_open()) {
		cout << "\n\n\t[Не найден файл с сообщением]\n" << endl;
		system("pause"); 
		exit(-1);
	}

	Init_S_box(FirstInitSbox);

	while (!in.eof()) {
		unsigned char X[8]{};
		in.read((char*)X, sizeof(X));
		long long temp = *(long long*)X;

		Shifr(X);

		*(long long*)H = *(long long*)Y ^ temp;
	}
	in.close();
}

unsigned int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char Choice{};

	do {
		system("cls");
		cout << "\n\t\t>> Лабораторная работа №3 (Цифровая подпись) <<\n\n";
		cout << "\tВыберите действие: 1 - Сгенерировать, 2 - Проверить: ";
		Choice = _getche();
	} while (Choice != '1' && Choice != '2');
	
	switch (Choice)
	{
		case '1':
		{
			do {
				GetHashSumm(FirstInitSbox++);
			} while (*(long long*)H % q == 0);

			int r, s, 
				h = *(long long*)H % q;				// H % q т.к. 0 < h < q

			srand(time(NULL));
			while(true) {
				int k = rand() % (q - 1) + 1;		// выбираем случайное k, 0 < k < q

				r = FastPower(a, k, p) % q;			// вычисляем подпись сообщения - (r, s)

				s = (k * h % q + x * r % q) % q;

				if (r && s) break;
			}
			
			ofstream out(FileName + ".sign", ios::out | ios::binary);
			if (!out.is_open()) {
				cout << "\n\n\t[Невозможно открыть файл: " + FileName + ".sign]\n" << endl;
				system("pause"); 
				exit(-1);
			}

			out << "{ " << r << ", " << s << " } Is Message Signature";

			out.close();

			cout << "\n\n\t[Сохранено в файл: " + FileName + ".sign]\n\n";
		}
		break;

		case '2': 
		{
			do {
				GetHashSumm(FirstInitSbox++);
			} while (*(long long*)H % q == 0);

			int r, s; char ch;

			ifstream in(FileName + ".sign", ios::in | ios::binary);
			if (!in.is_open()) {
				cout << "\n\n\t[Не найден файл с подписью: " + FileName + ".sign]\n" << endl;
				system("pause"); 
				exit(-1);
			}

			in >> ch >> r >> ch >> s;

			in.close();

			int w = InversionByModule (*(long long*)H % q, q);
			int U1 = s * w % q;
			int U2 = (q-r) * w % q;
			int U = (FastPower(a, U1, p) * FastPower(y, U2, p)) % p % q;

			cout << "\n\n\tU = " << U << ", r = " << r << " | Подпись";

			if (U == r) cout << " верна.\n\n"; else cout << " не верна.\n\n";
		}
		break;
	}
	system("pause");
	return 0;
}