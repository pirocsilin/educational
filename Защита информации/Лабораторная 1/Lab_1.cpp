
// ------------------------------ Шифры с открытым ключом ----------------------------

#include <iostream>
#include <Windows.h>
#include <iomanip>
#include <cmath>
using namespace std;

int FastPower(int a, int x, int p) {

    int LenBin = log2(x) + 1, y = 1, s = a;

    while (LenBin--) {
        if (x & 1) y = y * s % p;   // если младший бит=1
        s = s * s % p;              
        x >>= 1;                    // двигаем степень на один разряд вправо
    }
    return y;
}

int NOD(int a, int b) {

    if (a < b) { swap(a, b); }

    int temp;
    while (b != 0){
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
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

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    system("mode con cols=85 lines=25");

    cout << '\n' << setw(57) << ">> ЛАБОРАТОРНАЯ РАБОТА №1 << \n" << endl;

    cout << "\n\t1. Cистема Диффи-Хеллмана:" << endl;
    cout << "\t   Параметры: p = 30803, g = 2, Xa = 1000, Xb = 2000;" << endl;
    int Ya = FastPower(2, 1000, 30803), 
        Yb = FastPower(2, 2000, 30803),
        Zab = FastPower(Yb, 1000, 30803),
        Zba = FastPower(Ya, 2000, 30803);
    printf( "\t   Ya = %d, Yb = %d, Zab = %d, Zba = %d.\n", Ya, Yb, Zab, Zba);

    cout << "\n\t2. шифр Шамира:" << endl;
    cout << "\t   Параметры: p = 30803, g = 2, cA = 501, cB = 601, m = 11111;" << endl;
    int dA = InversionByModule(501, 30802),
        dB = InversionByModule(601, 30802),
        x1 = FastPower(11111, 501, 30803),
        x2 = FastPower(x1, 601, 30803),
        x3 = FastPower(x2, dA, 30803), 
        x4 = FastPower(x3, dB, 30803);
    printf("\t   dA = %d, dB = %d, x1 = %d, x2 = %d, x3 = %d, x4 = %d.\n", dA, dB, x1, x2, x3, x4);
    
    cout << "\n\t3. шифр Эль-Гамаля:" << endl;
    cout << "\t   Параметры: p = 30803, g = 2, c = 500, k = 600, m = 11111;" << endl;
    int d = FastPower(2, 500, 30803),
        r = FastPower(2, 600, 30803),
        e = 11111 * FastPower(d, 600, 30803) % 30803,
        m = e * FastPower(r, 30803-1-500, 30803) % 30803;
    printf("\t   d = %d, r = %d, e = %d, m' = %d.\n", d, r, e, m);

    cout << "\n\t4. шифр RSA:" << endl;
    cout << "\t   Параметры: P = 131, Q = 227, d = 3, m = 11111;" << endl;
    int N = 131 * 227,
        f = 130 * 226,
        c = InversionByModule(3, f),
        E = FastPower(11111, 3, N),             // A encrypted message and send B
        M = FastPower(E, c, N);                 // B recive message and decrypt him
    printf("\t   e = %d, m' = %d.\n\n\n", E, M);

    system("pause");

    return 0;
}
