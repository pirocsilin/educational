#include "Ucomplex.h"

TComplex UserInputComplex() {
	TComplex UComplex;
	bool Correct = false;
	do {
		try {
			string UInput;
			cout << "\n\n Введите комплексное число (пример: -4+12i): ";
			getline(cin, UInput);
			UComplex = TComplex(UInput);
			Correct = true;
		}
		catch (TComplex::ErrComplex e) {
			cout << "\n ERROR: [" << e.msg << "]";
		}
		catch (...) {
			cout << "\n ERROR: [Неизвестная ошибка]" << endl;
		}
	} while (!Correct);

	return UComplex;
}

vector<TComplex> v(5);

template<typename T>
void PrinBin(const char * msg, T lambda, TComplex& op2) {
	cout << "\n" << msg;
	for_each(v.begin(), v.end(), lambda);
}

template<typename T>
void PrintUn(const char* msg, T lambda) {
	cout << "\n" << msg;
	for_each(v.begin(), v.end(), lambda);
}

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=120 lines=30");
	
	cout << "\n Массив случайных чисел: ";

	srand(time(0));
	for (int i = 0; i < 5; i++) {
		double Re = 20. - rand() % 41;
		double Im = 20. - rand() % 41;
		v[i] = TComplex(Re, Im);
		cout << setw(10) << v[i].GetComStr();
	}

	TComplex UC = UserInputComplex();
	cout << " Введено число: " << UC.GetComStr() << endl;

	cout << "\n Некоторые операции с числом и массивом:" << endl;

	try {
		PrinBin("\tСумма:    ", [&UC](TComplex& c) {cout << '\t' << (c + UC).GetComStr(); }, UC);
		PrinBin("\n\tРазность: ", [&UC](TComplex& c) {cout << '\t' << (c - UC).GetComStr(); }, UC);
		PrinBin("\n\tУмножение:", [&UC](TComplex& c) {cout << '\t' << (c * UC).GetComStr(); }, UC);
		PrinBin("\n\tДеление:  ", [&UC](TComplex& c) {cout << '\t' << (c / UC).GetComStr(); }, UC);
		PrinBin("\n\tРавно:    ", [&UC](TComplex& c) {cout << '\t' << (c == UC); }, UC);

		cout << "\n\n Некоторые унарные операции с массивом чисел:" << endl;

		PrintUn("\tОбратное:  ", [](TComplex& c) {cout << '\t' << (c.GetReverseC()).GetComStr(); });
		PrintUn("\n\tКвадрат: ", [](TComplex& c) {cout << '\t' << (c.GetSqrC()).GetComStr(); });
		PrintUn("\n\tМодуль:  ", [](TComplex& c) {cout << '\t' << c.GetModule(); });
		PrintUn("\n\t1-й корень 3-й степени: ", [](TComplex& c) {cout << '\t' << (c.Get_i_Root(3,1)).GetComStr(); });

	}
	catch (TComplex::ErrComplex e) {
		cout << "\n ERROR: [" << e.msg << "]" << endl;
	}
	catch (...) {
		cout << "\n ERROR: [Неизвестная ошибка]" << endl;
	}
	cout << endl << endl;
	system("pause");

	return 0;
}