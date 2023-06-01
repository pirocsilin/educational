#include "Ucomplex.h"

TComplex UserInputComplex() {
	TComplex UComplex;
	bool Correct = false;
	do {
		try {
			string UInput;
			cout << "\n\n ������� ����������� ����� (������: -4+12i): ";
			getline(cin, UInput);
			UComplex = TComplex(UInput);
			Correct = true;
		}
		catch (TComplex::ErrComplex e) {
			cout << "\n ERROR: [" << e.msg << "]";
		}
		catch (...) {
			cout << "\n ERROR: [����������� ������]" << endl;
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
	
	cout << "\n ������ ��������� �����: ";

	srand(time(0));
	for (int i = 0; i < 5; i++) {
		double Re = 20. - rand() % 41;
		double Im = 20. - rand() % 41;
		v[i] = TComplex(Re, Im);
		cout << setw(10) << v[i].GetComStr();
	}

	TComplex UC = UserInputComplex();
	cout << " ������� �����: " << UC.GetComStr() << endl;

	cout << "\n ��������� �������� � ������ � ��������:" << endl;

	try {
		PrinBin("\t�����:    ", [&UC](TComplex& c) {cout << '\t' << (c + UC).GetComStr(); }, UC);
		PrinBin("\n\t��������: ", [&UC](TComplex& c) {cout << '\t' << (c - UC).GetComStr(); }, UC);
		PrinBin("\n\t���������:", [&UC](TComplex& c) {cout << '\t' << (c * UC).GetComStr(); }, UC);
		PrinBin("\n\t�������:  ", [&UC](TComplex& c) {cout << '\t' << (c / UC).GetComStr(); }, UC);
		PrinBin("\n\t�����:    ", [&UC](TComplex& c) {cout << '\t' << (c == UC); }, UC);

		cout << "\n\n ��������� ������� �������� � �������� �����:" << endl;

		PrintUn("\t��������:  ", [](TComplex& c) {cout << '\t' << (c.GetReverseC()).GetComStr(); });
		PrintUn("\n\t�������: ", [](TComplex& c) {cout << '\t' << (c.GetSqrC()).GetComStr(); });
		PrintUn("\n\t������:  ", [](TComplex& c) {cout << '\t' << c.GetModule(); });
		PrintUn("\n\t1-� ������ 3-� �������: ", [](TComplex& c) {cout << '\t' << (c.Get_i_Root(3,1)).GetComStr(); });

	}
	catch (TComplex::ErrComplex e) {
		cout << "\n ERROR: [" << e.msg << "]" << endl;
	}
	catch (...) {
		cout << "\n ERROR: [����������� ������]" << endl;
	}
	cout << endl << endl;
	system("pause");

	return 0;
}