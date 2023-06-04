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

	cout << "\n Массив случайных чисел (b=16, c=5): ";

	srand(time(0));
	for (int i = 0; i < 5; i++) {
		double num = RAND_MAX / 2 - rand();
		double denom = rand() % 100 + 1;
		v[i] = TPNumber(num / denom, 16, 5);
		if (i == 2) N = v[i];
		cout << v[i].GetNumStr() << "  ";
	}
	cout << "\n\n Числа в десятичном представлении:   ";
	for_each(v.begin(), v.end(), [](TPNumber& n) {cout << n.GetNum() << "  "; });

	cout << "\n\n Выбрали из массива число для опреций: " << N.GetNumStr();

	cout << "\n\n Некоторые операции над массивом и числом: " << endl << endl;

	try {
		cout << "\tСложение:  "; 
		for_each(v.begin(), v.end(), SummObjs);

		cout << "\n\n\tВычитание: "; 
		for_each(v.begin(), v.end(), DiffObjs);

		cout << "\n\n\tУмножение: ";
		for_each(v.begin(), v.end(), MulObjs());

		cout << "\n\n\tДеление:   ";
		for_each(v.begin(), v.end(), [](TPNumber& n) {cout << (n / N).GetNumStr() << "  "; });
	}
	catch(TPNumber::TPNumErr err){
		cout << " ERROR: [" << err.msg << "]" << endl;
	}

	cout << endl << endl;
	system("pause");

	return 0;
}
