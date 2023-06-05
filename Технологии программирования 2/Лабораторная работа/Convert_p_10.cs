using System;

namespace Programming_Tech_Form {

    static class Convert_p_10 {

        // Преобразовать цифру в число.
        private static double Char_To_num(char ch) {

            return (ch >= '0' && ch <= '9') ? ch - 48 :
                   (ch >= 'A' && ch <= 'F') ? ch - 55 :
                   (ch >= 'a' && ch <= 'f') ? ch - 87 :
                   throw new Exception("Ошибка записи числа");
        }

        // Преобразовать строку в число
        private static double Convert(string P_num, int p, double k) {

            double rezult = 0;

            foreach (char val in P_num) {
                rezult += Char_To_num(val) * Math.Pow(p, k--);
            }

            return rezult;
        }

        // Преобразовать из с.сч. с основанием р в с.сч. с основанием 10
        public static double Dval(string P_num, int p) {

            if (p < 2 || p > 16) {
                throw new Exception("Основание должно быть в диапазоне 2-16");
            }

            int sign = P_num[0] is '-' ? -1 : 1;

            P_num = P_num.TrimStart('-');
            
            double k = -1;

            for (int i = P_num.Length - 1; i >= 0; i--) {
                k = P_num[i] == ',' ? -1 : k + 1;

                if(P_num[i] != ',' && Char_To_num(P_num[i]) >= p) {
                    throw new Exception("Число не соответствует основанию системы");
                }
            }

            return sign * Convert(P_num.Replace(",", ""), p, k);
        }
    }
}
