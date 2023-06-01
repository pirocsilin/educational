#include <iostream>
#include <conio.h>
#include <iomanip>
#include <math.h>

using namespace std;

double  X_0[] = { 3, 4 },
        X_Next[] = { 0, 0 },
        Lambda = 0.001,
        Target = 0.0001,
        Alfa1 = 0,
        Alfa2 = 0;

double G1_X(double x[2]) { return 60 - (5 * x[0] + 6 * x[1]); }         // ограничение 1
double G2_X(double x[2]) { return 72 - (4 * x[0] + 9 * x[1]); }         // ограничение 2

double FX(double x[2]) { return - pow(x[0]-9, 2) - pow(x[1]-6, 2); }    // целевая функция

double FX1()  { return -2 * X_0[0] + 18; }  // частные производные от целевой ф-ии
double FX2()  { return -2 * X_0[1] + 12; }

double G1_X1 = -5;                          // частные производные от ф-й ограничений
double G1_X2 = -6;
double G2_X1 = -4;
double G2_X2 = -9;


int main()
{
    setlocale(LC_ALL, "Rus");
    int i = 1, IndexLastPtrTrue = 0;
    double FXLastPtrTrue = FX(X_0); 

    cout << '\n' << setw(50) << ">> ЛАБОРАТОРНАЯ РАБОТА №3 <<";

    while (true)
    {
        cout << "\n\n Итерация " << i;

        X_Next[0] = max(0., X_0[0] + Lambda * (FX1() + Alfa1 * G1_X1 + Alfa2 * G2_X1));
        X_Next[1] = max(0., X_0[1] + Lambda * (FX2() + Alfa1 * G1_X2 + Alfa2 * G2_X2));

        if (G1_X(X_Next) >= 0 && G2_X(X_Next) >= 0)
        {
            printf("\n G1(X%d) = %.3lf > 0", i, G1_X(X_Next));
            printf("\n G2(X%d) = %.3lf > 0 => т.X%d = [%.3lf;%.3lf] принадлежит ОДР", 
                i, G2_X(X_Next), i, X_Next[0], X_Next[1]);

            cout << "\n Проверка достижения требуемой точности:";
            double Accuracy = abs(FX(X_Next) - FXLastPtrTrue);

            if (Accuracy > Target)
            {
                printf("\n |F(X%d) - F(X%d)| = |%.4lf - %.4lf| = %.4lf > %.4f", 
                    i, IndexLastPtrTrue, FX(X_Next), FXLastPtrTrue, Accuracy, Target);
                cout << "\n Точность не достигнута!";

                FXLastPtrTrue = FX(X_Next);
                IndexLastPtrTrue = i;
                Alfa1 = Alfa2 = 0;
            }

            else
            {
                printf("\n |F(X%d) - F(X%d)| = |%.5lf - %.5lf| = %.5lf < %.4f", 
                    i, IndexLastPtrTrue, FX(X_Next), FXLastPtrTrue, Accuracy, Target);
                cout << "\n Точность достигнута.";

                break;
            }
        }
        else
        {
            if (G1_X(X_Next) < 0)
            {
                printf("\n G1(X%d) = %.3lf < 0 => т.X%d = [%.3lf;%.3lf] не принадлежит ОДР", 
                    i, G1_X(X_Next), i, X_Next[0], X_Next[1]);

                Alfa1 = Alfa1 - Lambda * G1_X(X_Next);
                printf("\n Alfa1 для X%d = %.5lf", i + 1, Alfa1);
            }
            else Alfa1 = 0;

            if (G2_X(X_Next) < 0)
            {
                printf("\n G2(X%d) = %.3lf < 0 => т.X%d = [%.3lf;%.3lf] не принадлежит ОДР", 
                    i, G2_X(X_Next), i, X_Next[0], X_Next[1]);

                Alfa2 = Alfa2 - Lambda * G2_X(X_Next);
                printf("\n Alfa2 для X%d = %.5lf", i + 1, Alfa2);
            }
            else Alfa2 = 0;
        }

        X_0[0] = X_Next[0];
        X_0[1] = X_Next[1];
        
        i++;
    }
    cout << "\n\n Любая клавиша - Выход "; 
    auto Exit = _getch();
    
    return 0;
}
