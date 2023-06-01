#include "TPNumber.h"

vector<TPNumber> v(5);
TPNumber N;

void SummObjs(TPNumber & n) {
	cout << (n + N).GetNumStr() << "  ";
}

void DiffObjs(TPNumber& n) {
	cout << (n - N).GetNumStr() << "  ";
}

class MulObjs {
public:
	void operator()(TPNumber& n) {
		cout << (n * N).GetNumStr() << "  ";
	}
};

int main() {

	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=95 lines=25");

	cout << "\n ������ ��������� ����� (b=16, c=5): ";

	srand(time(0));
	for (int i = 0; i < 5; i++) {
		double num = RAND_MAX / 2 - rand();
		double denom = rand() % 100 + 1;
		v[i] = TPNumber(num / denom, 16, 5);
		if (i == 2) N = v[i];
		cout << v[i].GetNumStr() << "  ";
	}
	cout << "\n\n ����� � ���������� �������������:   ";
	for_each(v.begin(), v.end(), [](TPNumber& n) {cout << n.GetNum() << "  "; });

	cout << "\n\n ������� �� ������� ����� ��� �������: " << N.GetNumStr();

	cout << "\n\n ��������� �������� ��� �������� � ������: " << endl << endl;

	try {
		cout << "\t��������:  "; 
		for_each(v.begin(), v.end(), SummObjs);

		cout << "\n\n\t���������: "; 
		for_each(v.begin(), v.end(), DiffObjs);

		cout << "\n\n\t���������: ";
		for_each(v.begin(), v.end(), MulObjs());

		cout << "\n\n\t�������:   ";
		for_each(v.begin(), v.end(), [](TPNumber& n) {cout << (n / N).GetNumStr() << "  "; });
	}
	catch(TPNumber::TPNumErr err){
		cout << " ERROR: [" << err.msg << "]" << endl;
	}

	cout << endl << endl;
	system("pause");

	return 0;
}
