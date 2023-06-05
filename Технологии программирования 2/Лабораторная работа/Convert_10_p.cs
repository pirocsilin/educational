using System;

namespace Programming_Tech_Form {

    static class Convert_10_p {

        //Преобразовать целое в символ.
        public static char Int_to_Char(int n){

            if (n < 0 || n > 15){
                throw new Exception("Digit must between 0-15");
            }

            return n == 10 ? 'A' : n == 11 ? 'B' :
                   n == 12 ? 'C' : n == 13 ? 'D' :
                   n == 14 ? 'E' : n == 15 ? 'F' :
                   (char)(n + 48);
        }

        //Преобразовать десятичное целое в с.сч. с основанием р.
        public static string Int_to_P(int n, int p){

            string rezult = "";

            if (p < 2 || p > 16){
                throw new Exception("Основание должно быть в диапазоне 2-16");
            }

            int sign = n >> 31 & 1;     // Запоминаем знак числа
            n = Math.Abs(n);

            while (n >= p){
                rezult = Int_to_Char(n % p) + rezult;
                n /= p;
            }
            rezult = Int_to_Char(n) + rezult;

            return sign == 1 ? '-' + rezult : rezult;
        }

        //Преобразовать десятичную дробь в с.сч. с основанием р.
        public static string Flt_to_P(double n, int p, int c){

            string rezult = "";

            if (c < 0 || c > 10){
                throw new Exception("Точность должна быть в диапазоне 1-10");
            }

            if (p < 2 || p > 16) {
                throw new Exception("Основание должно быть в диапазоне 2-16");
            }

            // Приводим число к нужной точности
            n = Math.Round(n * Math.Pow(10, c)) / Math.Pow(10, c);

            // Работаем с дробной частью
            double fract = Math.Abs(n) - Math.Abs((int)n);

            while (c-- > 0 && fract != 0){
                fract *= p;
                rezult += Int_to_Char((int)fract);
                fract -= (int)fract;
            }

            rezult = (Int_to_P((int)n, p) + (',' + rezult).Trim('0')).Trim(',');

            return n < 0 && (int)n == 0 ? '-' + rezult : rezult;
        }

        //Преобразовать десятичное 
        //действительное число в с.сч. с осн. p.
        public static string Do(double n, int p, int c){

            return Flt_to_P(n, p, c);
        }
    }
}
