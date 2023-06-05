using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace TI_Lab_1 {

    enum FileType { un_distrib, not_un_distrib }
    enum TextType { one_symbol, group_symbols }

    static class Entropy {

        public static double GetMax(double n) => Math.Log2(n);

        public static double GetValue(double[] arr, int c) {
            double H = 0;
            foreach (var val in arr) {
                H += val / c * Math.Log2(val / c);
            }
            return -H;
        }

        public static double GetValue(string fName, TextType textType, int Part = 1) {

            Dictionary<string, double> dic = new Dictionary<string, double>();
            int Count = 0, Next_Part = 0, i = -1;
            string sKey = "", s;
            StreamReader reader;
            char Key;

            try {
                switch (textType) {

                    case TextType.one_symbol:
                        using (reader = new StreamReader(fName)) {
                            while (reader.Peek() != -1) {
                                Key = (char)reader.Read();
                                if (Key >= 1072 && Key <= 1103 || Key == 32 || Key == 'ё' || Key == 'Ё') {
                                    Key = Key == 'ё' || Key == 'Ё' ? 'е' :
                                          Key == 'ъ' || Key == 'Ъ' ? 'ь' : Key;        // 'Ъ' - ну вдруг :)
                                    Count++;
                                    if (dic.ContainsKey(Key.ToString().ToLower()))
                                        dic[Key.ToString().ToLower()]++;
                                    else
                                        dic.Add(Key.ToString().ToLower(), 1);
                                }
                            }
                        }
                        break;

                    case TextType.group_symbols:
                        using (reader = new StreamReader(fName)) { s = reader.ReadToEnd(); }
                        while (++i < s.Length) {
                            Key = s[i];
                            if (Key >= 'А' && Key <= 'я' || Key == 32 || Key == 'ё' || Key == 'Ё') {
                                Key = Key == 'ё' || Key == 'Ё' ? 'е' :
                                      Key == 'ъ' || Key == 'Ъ' ? 'ь' : Key;
                                Next_Part = sKey == "" ? i : Next_Part;
                                sKey += Key;
                                sKey = sKey.ToLower();
                                if (sKey.Length == Part) {
                                    Count++; i = Next_Part;
                                    if (dic.ContainsKey(sKey))
                                        dic[sKey]++;
                                    else
                                        dic.Add(sKey, 1);
                                    sKey = "";
                                }
                            }
                        }
                        break;
                }
            }
            catch (FileNotFoundException) {
                Console.WriteLine($"\n ERROR: Файл {fName} не найден!");
                Console.Write("\n Press any key for exit .. ");
                Console.ReadKey();
                Environment.Exit(0);
            }

            return GetValue(dic.Values.ToArray(), Count);
        }
    }

    class Program {

        static void Main(string[] args) {

            string File_1 = "uniform_distribution.txt",
                   File_2 = "not_uniform_distribution.txt",
                   File_3 = "War_And_Peace_Tom2.txt";

            CreateFile(new char[] { 'а', 'б', 'в', 'г', 'д' }, FileType.un_distrib, File_1);
            CreateFile(new char[] { 'а', 'б', 'в', 'г', 'д' }, FileType.not_un_distrib, File_2);

            Console.WriteLine($"\n{"Оценка энтропии:",18}{"один_символ",14}{"пары",10}{"тройки",10}" +
                              $"{"четверки",10}{"пятёрки",10}{"H_теор.",10}");

            // Равномерное распределение символов
            Console.Write($"\n{"файл_1:",18}{Entropy.GetValue(File_1, TextType.one_symbol),14:F4}");
            for (int i = 2; i <= 5; i++) {
                Console.Write($"{Entropy.GetValue(File_1, TextType.group_symbols, i) / i,10:F4}");
            }
            Console.Write($"{Entropy.GetMax(5),10:F4}\n");

            // Не равномерное распределение символов
            Console.Write($"\n{"файл_2:",18}{Entropy.GetValue(File_2, TextType.one_symbol),14:F4}");
            for (int i = 2; i <= 5; i++) {
                Console.Write($"{Entropy.GetValue(File_2, TextType.group_symbols, i) / i,10:F4}");
            }
            Console.Write($"{Entropy.GetValue(new double[] { 0.4, 0.2, 0.2, 0.1, 0.1 }, 1),10:F4}\n");

            // Энтропия текста
            Console.Write($"\n{"худ-й текст:",18}{Entropy.GetValue(File_3, TextType.one_symbol),14:F4}");
            for (int i = 2; i <= 5; i++) {
                Console.Write($"{Entropy.GetValue(File_3, TextType.group_symbols, i) / i,10:F4}");
            }
            double[] P_Simbols_in_Text = { 0.175, 0.092, 0.073, 0.063, 0.063, 0.053, 0.045, 0.045,
                                           0.040, 0.038, 0.035, 0.028, 0.028, 0.025, 0.023, 0.021,
                                           0.018, 0.016, 0.016, 0.014, 0.014, 0.013, 0.012, 0.010,
                                           0.009, 0.007, 0.006, 0.006, 0.004, 0.003, 0.003, 0.002 };

            Console.Write($"{Entropy.GetValue(P_Simbols_in_Text, 1),10:F4}\n\n");

            Console.Write("\n Press any key for exit .. ");
            Console.ReadKey();
        }

        static void CreateFile(char[] A, FileType ft, string fName) {

            Random rand = new Random();
            int ctr = 20000;
            switch (ft) {
                case FileType.un_distrib:
                    using (StreamWriter writer = new StreamWriter(fName)) {
                        while (ctr-- > 0) {
                            writer.Write(A[rand.Next(A.Length)]);
                            if (ctr % 50 == 0) writer.Write('\n');
                        }
                    }
                    break;

                case FileType.not_un_distrib:
                    using (StreamWriter writer = new StreamWriter(fName)) {
                        while (ctr-- > 0) {
                            double val = rand.NextDouble();
                            writer.Write(val >= 0.0 && val < 0.4 ? A[0] :
                                         val >= 0.4 && val < 0.6 ? A[1] :
                                         val >= 0.6 && val < 0.8 ? A[2] :
                                         val >= 0.8 && val < 0.9 ? A[3] :
                                                                   A[4]);
                            if (ctr % 50 == 0) writer.Write('\n');
                        }
                    }
                    break;
            }
        }
    }
}

