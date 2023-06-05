using System;
using System.IO;
using System.Linq;
using System.Collections.Generic;

namespace TI_Lab_3 {

    static class Coding {

        public static Dictionary<string, string> Code_Words = new Dictionary<string, string>();
        public static string File_As_String, Bit_Sequence;
        public static int CS;

        // Кодирование файла
        public static void Encode(string fName, string extension) {

            // Записываем в строку File_As_String все символы из файла
            using (StreamReader reader = new StreamReader(fName)) { File_As_String = reader.ReadToEnd(); }
            Bit_Sequence = "";
            int i = -1;
            char Key;

            // Представляем строку File_As_String как последовательность 0 и 1
            CS = 0;
            while (++i < File_As_String.Length) {
                Key = File_As_String[i];
                if (Key >= 'А' && Key <= 'я' || Key == 32 || Key == 'ё' || Key == 'Ё') {
                    Key = Key == 'ё' || Key == 'Ё' ? 'е' :
                          Key == 'ъ' || Key == 'Ъ' ? 'ь' : Key;
                    Bit_Sequence += Code_Words[Key.ToString().ToLower()];
                    CS++;
                }
            }

            // Используем последовательность Bit_Sequence для кодирования файла
            byte Coun_Bit = 0;
            byte Byte_For_Write = 0;
            string f = fName.Replace(".txt", extension);
            using (BinaryWriter writer = new BinaryWriter(File.Open(f, FileMode.OpenOrCreate))) {
                foreach (var v in Bit_Sequence) {
                    byte NextBit = v == '0' ? (byte)0 : (byte)1;
                    Byte_For_Write = (byte)(Byte_For_Write | NextBit);

                    if (++Coun_Bit == 8) {
                        writer.Write(Byte_For_Write);               // Запись байта данных в файл
                        Coun_Bit = 0;
                        Byte_For_Write = 0;
                    }
                    Byte_For_Write = (byte)(Byte_For_Write << 1);
                }
                if (Coun_Bit != 0) {
                    Byte_For_Write = (byte)(Byte_For_Write << 7 - Coun_Bit);
                    writer.Write(Byte_For_Write);
                }
            }
        }

        // Декодирование файла (вывод в консоль для проверки кодирования)
        public static void Decode(string fName) {
            Dictionary<string, string> bin = new Dictionary<string, string>();
            foreach (var v in Coding.Code_Words)
                bin[v.Value] = v.Key;

            int CountSymbols = 0;
            byte Byte_For_Read;
            string Code = "";
            using (BinaryReader reader = new BinaryReader(File.Open(fName, FileMode.Open), System.Text.Encoding.ASCII)) {
                while (reader.PeekChar() > -1) {
                    Byte_For_Read = reader.ReadByte();
                    for (int i = 7; i >= 0; i--) {
                        Code += (Byte_For_Read >> i & 1) == 0 ? '0' : '1';
                        if (bin.ContainsKey(Code) && ++CountSymbols <= CS) {
                            Console.Write(bin[Code]);
                            Code = "";
                        }
                    }
                }
            }
        }

        // Определяем энтропию записанной битовой последовательности 
        public static double GetEntropy(int Part) {
            Dictionary<string, double> dic = new Dictionary<string, double>();
            Dictionary<string, string> bin = new Dictionary<string, string>();
            foreach (var v in Coding.Code_Words) bin[v.Value] = v.Key;

            int ptr = 0, CountParts = 0, i = -1;
            string Key = "", sKey = "";

            while (++i < Bit_Sequence.Length) {
                Key += Bit_Sequence[i];
                if (bin.ContainsKey(Key)) {
                    sKey += bin[Key];
                    Key = "";
                    if (sKey.Length == 1)
                        ptr = i;
                    if (sKey.Length == Part) {
                        CountParts++; i = ptr;
                        if (dic.ContainsKey(sKey))
                            dic[sKey]++;
                        else
                            dic.Add(sKey, 1);
                        sKey = "";
                    }
                }
            }

            double H = 0;
            foreach (var val in dic.Values.ToArray()) {
                H += val / CountParts * Math.Log2(val / CountParts);
            }
            return -H;
        }
    }

    public class Tree {
        public Tree left = null;
        public Tree right = null;
        public string Data;
        public string Code;

        public Tree(string data, string c = "") { Data = data; Code = c; }

        public void AddNode(string data, byte bit) {

            if (this.left == null && bit == 0)
                this.left = new Tree(data, this.Code + '0');

            else if (this.right == null && bit == 1)
                this.right = new Tree(data, this.Code + '1');

            else if (this.left.Data.Contains(data))
                this.left.AddNode(data, bit);

            else if (this.right.Data.Contains(data))
                this.right.AddNode(data, bit);
        }

        public static void Get_Code_Words(Tree ptr) {
            if (ptr != null) {
                Get_Code_Words(ptr.left);
                if (ptr.Data.Length == 1)
                    Coding.Code_Words[ptr.Data] = ptr.Code;
                Get_Code_Words(ptr.right);
            }
        }
    }

    public static class Program {

        public static Tree bTree;
        public static Dictionary<string, double> dic = new Dictionary<string, double>();

        class KeyComparer : IComparer<string> {
            public int Compare(string a, string b) {
                return dic[a] - dic[b] > 0 ? -1 : dic[a] - dic[b] < 0 ? 1 : 0;
            }
        }

        static void Huffman(int n, string[] P) {
            if (n == 1) {
                bTree = new Tree(P[0] + P[1]);
                bTree.AddNode(P[0], 0);
                bTree.AddNode(P[1], 1);
            }
            else {
                string Seq_Prev = P[n - 1],
                       Seq_Last = P[n];
                double Q = dic[Seq_Prev] + dic[Seq_Last];
                string NewSeq = Seq_Prev + Seq_Last;
                P[n - 1] = NewSeq;
                dic[NewSeq] = Q;

                Array.Sort(P, new KeyComparer());

                Huffman(n - 1, P);

                bTree.AddNode(Seq_Prev, 0);
                bTree.AddNode(Seq_Last, 1);
            }
        }

        public static void Shennon(string[] P) {
            double[] Q = new double[P.Length];
            double[] L = new double[P.Length];
            Q[0] = 0;
            L[0] = -Math.Floor(Math.Log2(dic[P[0]]));
            for (int i = 1; i < Q.Length; i++) {
                Q[i] = Q[i - 1] + dic[P[i - 1]];
                L[i] = -Math.Floor(Math.Log2(dic[P[i]]));
            }
            for (int i = 0; i < P.Length; i++) {
                for (int j = 0; j < L[i]; j++) {
                    Q[i] *= 2;
                    Coding.Code_Words[P[i]] += Math.Floor(Q[i]).ToString();
                    if (Q[i] > 1) Q[i] -= 1;
                }
            }
        }

        public static void Fano(int L, int R, string[] P) {
            if (L < R) {
                int m = Med(L, R, P);
                for (int i = L; i <= R; i++) {
                    if (i <= m)
                        Coding.Code_Words[P[i]] += '0';
                    else
                        Coding.Code_Words[P[i]] += '1';
                }
                Fano(L, m, P);
                Fano(m + 1, R, P);
            }
        }

        public static int Med(int L, int R, string[] P) {

            double SumL = 0;
            for (int i = L; i < R; i++)
                SumL += dic[P[i]];
            double SumR = dic[P[R]];
            int m = R;
            while (SumL >= SumR) {
                m--;
                SumL -= dic[P[m]];
                SumR += dic[P[m]];
            }
            return m;
        }

        public static void Main() {

            Console.Write($"\n{"Оценки:",19}{"Избыт-ть",12}{"один_симв",12}{"H_пары",12}" +
                            $"{"H_тройки",12}{"H_четв-и",12}{"H_пятёрки",12}");

            int ctr = 0;
            foreach (var fName in new string[]{"uniform_distribution.txt",
                                               "not_uniform_distribution.txt",
                                               "War_And_Peace_Tom2.txt" }) {

                int Count_Symbols = Read_File(fName);                           // Получаем элементы и их количество из файла
                foreach (var v in dic) dic[v.Key] = v.Value / Count_Symbols;    // Считаем частоту вхождения каждого элемента
                string[] P = dic.Keys.ToArray();                                // Составляем список элементов
                string[] tmp = dic.Keys.ToArray();
                Array.Sort(P, new KeyComparer());                               // Сортируем элементы по убыванию частот
                Huffman(P.Length - 1, P);                                       // Генерируем кодовые слова для каждого элемента
                Tree.Get_Code_Words(bTree);                                     // Формуруем таблицу кодовых слов
                Coding.Encode(fName, "_Haffman.dat");                           // Кодируем файл

                string f = ++ctr == 1 ? "файл_1:" : ctr == 2 ? "файл_2:" : "худ-й текст:";
                if(ctr == 1) 
                    Console.Write($"\n\n [Хаффман]  {f}");
                else
                    Console.Write($"\n\n{f,19}");
                for (int i = 1; i <= 5; i++) {
                    double H = Coding.GetEntropy(i);
                    if (i == 1) {                                               // Определяем избыточность кодирования
                        double L_avrg = 0;
                        foreach (var v in tmp) {
                            L_avrg += dic[v] * Coding.Code_Words[v].Length;
                        }
                        Console.Write($"{L_avrg - H,12:F4}");
                    }
                    Console.Write($"{H / i,12:F4}");
                }

                //Console.WriteLine(CheckWords());                              // Проверка префиксного кода
                dic.Clear();
            }
            ctr = 0;
            foreach (var fName in new string[]{"uniform_distribution.txt",
                                               "not_uniform_distribution.txt",
                                               "War_And_Peace_Tom2.txt" }) {

                Coding.Code_Words.Clear();
                int Count_Symbols = Read_File(fName);
                foreach (var v in dic) {
                    dic[v.Key] = v.Value / Count_Symbols;
                    Coding.Code_Words.Add(v.Key, "");
                }
                string[] P = dic.Keys.ToArray();
                string[] tmp = dic.Keys.ToArray();
                Array.Sort(P, new KeyComparer());
                
                Shennon(P);                                                     // Генерируем кодовые слова методом Шеннона
                Coding.Encode(fName, "_Shennon.dat");                           // Кодируем файл

                string f = ++ctr == 1 ? "файл_1:" : ctr == 2 ? "файл_2:" : "худ-й текст:";
                if (ctr == 1)
                    Console.Write($"\n\n\n [Шеннон]   {f}");
                else
                    Console.Write($"\n\n{f,19}");
                for (int i = 1; i <= 5; i++) {
                    double H = Coding.GetEntropy(i);
                    if (i == 1) {                                               // Определяем избыточность кодирования
                        double L_avrg = 0;
                        foreach (var v in tmp) {
                            L_avrg += dic[v] * Coding.Code_Words[v].Length;
                        }
                        Console.Write($"{L_avrg - H,12:F4}");
                    }
                    Console.Write($"{H / i,12:F4}");
                }

                //Console.WriteLine(CheckWords());                              // Проверка префиксного кода
                dic.Clear();
            }

            ctr = 0;
            foreach (var fName in new string[]{"uniform_distribution.txt",
                                               "not_uniform_distribution.txt",
                                               "War_And_Peace_Tom2.txt" }) {

                Coding.Code_Words.Clear();
                int Count_Symbols = Read_File(fName);
                foreach (var v in dic) {
                    dic[v.Key] = v.Value / Count_Symbols;
                    Coding.Code_Words.Add(v.Key, "");
                }
                string[] P = dic.Keys.ToArray();
                string[] tmp = dic.Keys.ToArray();
                Array.Sort(P, new KeyComparer());

                Fano(0, P.Length - 1, P);                                       // Генерируем кодовые слова методом Фано
                Coding.Encode(fName, "_Fano.dat");                              // Кодируем файл

                string f = ++ctr == 1 ? "файл_1:" : ctr == 2 ? "файл_2:" : "худ-й текст:";
                if (ctr == 1)
                    Console.Write($"\n\n\n [Фано]     {f}");
                else
                    Console.Write($"\n\n{f,19}");
                for (int i = 1; i <= 5; i++) {
                    double H = Coding.GetEntropy(i);
                    if (i == 1) {                                               // Определяем избыточность кодирования
                        double L_avrg = 0;
                        foreach (var v in tmp) {
                            L_avrg += dic[v] * Coding.Code_Words[v].Length;
                        }
                        Console.Write($"{L_avrg - H,12:F4}");
                    }
                    Console.Write($"{H / i,12:F4}");
                }

                //Console.WriteLine(CheckWords());                              // Проверка префиксного кода
                dic.Clear();
            }

            Console.WriteLine('\n');
            Console.Write("\n  Press any key for exit .. ");
            Console.ReadKey();

        }
        static bool CheckWords() {
            foreach (var word in Coding.Code_Words.Values)
                foreach (var prefix in Coding.Code_Words.Values)
                    if (word != prefix && word.IndexOf(prefix) == 0) {
                        return false;
                    }
            return true;
        }

        static int Read_File(string fName) {

            int Count = 0;
            char Key;
            try {
                using (StreamReader reader = new StreamReader(fName)) {
                    while (reader.Peek() != -1) {
                        Key = (char)reader.Read();
                        if (Key >= 'А' && Key <= 'я' || Key == 32 || Key == 'ё' || Key == 'Ё') {
                            Key = Key == 'ё' || Key == 'Ё' ? 'е' :
                                  Key == 'ъ' || Key == 'Ъ' ? 'ь' : Key;
                            Count++;
                            if (dic.ContainsKey(Key.ToString().ToLower()))
                                dic[Key.ToString().ToLower()]++;
                            else
                                dic.Add(Key.ToString().ToLower(), 1);
                        }
                    }
                }
            }
            catch (FileNotFoundException) {
                Console.WriteLine($"\n\n ERROR: Файл {fName} не найден!");
                Console.Write("\n Press any key for exit .. ");
                Console.ReadKey();
                Environment.Exit(0);
            }
            return Count;
        }
    }
}