#include "UFrac.h"

TFract InputUserFract() {
	const char* err = "Ошибка ввода данных";
	TFract UserFract;
	bool wrong = true;
	string rez;
	int a, b;
	char ch;

	do {
		cout << "\n Введите дробь (пример 2/3): ";
		getline(cin, rez);
		
		try {
			istringstream is(rez);
			is >> a >> ch >> b;
			if (ch != '/') {
				throw err;
			}

			UserFract = TFract(a, b);
			wrong = false;
		}
		catch (TFract::ErrFract e) {
			cout << "ERROR: [" << e.msg << "]" << endl;
		}
		catch(const char*){
			cout << "ERROR: [" << err << "]" << endl;
		}
	} while (wrong);

	cout << " Введена дробь: " << UserFract.Display() << endl;

	return UserFract;
}

template <class T>
void PrintPtr(T* ptr) {
	cout << ptr->Display() <<" ";
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	system("mode con cols=110 lines=25");
	cout << endl << setw(70) << ">> Лабораторная работа №1 <<\n" << endl;

	vector<TFract*> Arr;
	vector<TFract*>::iterator it;

	srand(time(0));
	for (int i = 0; i < 10; i++) {
		int a = rand() % 100 + 1;
		int b = rand() % 100 + 1;
		Arr.push_back(new TFract(a, b));
	}
	cout << "\n Сгенерировали 10 простых дробей: ";
	
	for_each(Arr.begin(), Arr.end(), PrintPtr<TFract>);

	TFract UFract = InputUserFract();

	cout << "\n Некоторые операции с пользовательской дробью и массивом: ";

	try {
		cout << "\n\n  Сложение:" << setw(10);
		for (it = Arr.begin(); it != Arr.end(); it++) {
			cout << (*(*it) + UFract).Display() << setw(9);
		}
		cout << "\n  Вычитание:" << setw(9);
		for (it = Arr.begin(); it != Arr.end(); it++) {
			cout << (*(*it) - UFract).Display() << setw(9);
		}
		cout << "\n  Умножение:" << setw(9);
		for (it = Arr.begin(); it != Arr.end(); it++) {
			cout << (*(*it) * UFract).Display() << setw(9);
		}
		cout << "\n  Деление:" << setw(11);
		for (it = Arr.begin(); it != Arr.end(); it++) {
			cout << (*(*it) / UFract).Display() << setw(9);
		}
	}
	catch (TFract::ErrFract e) {
		cout << "ERROR: [" << e.msg << "]" << endl;
	}

	cout << endl << endl;
	system("pause");
}