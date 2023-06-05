using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;

namespace TLP_Projects {

    class Program {

        class Config {

            public struct Condition {
                public string q, a, t;  // q∈Q, a∈V, t∈Z
                public Condition(string x, string y, string z) { q = x; a = y; t = z; }
                public string GetCond() => $"({q}, {a}, {t})";
            }

            public struct Rule {
                public string _q, _g, p;   // _q∈Q,  _g∈Z*
                public Rule(string _q, string _g, string p) { this._q = _q; this._g = _g; this.p = p; }
                public string GetRule() => $"({_q}, {_g}, {p})";
            }

            public Condition c;
            public Rule r;

            static public int i;
            static public string W;
            static public string q_0;
            static public string z_0;
            static public Stack<string> Z;
            static public string Translat;
            static public Dictionary<Condition, Rule> Q;
            static public HashSet<string> q = new HashSet<string>();    // Множество состояний автомата 
            static public HashSet<string> F = new HashSet<string>();    // Множество конечных состояний 
            static public HashSet<string> V = new HashSet<string>();    // Входной Алфавит              
            static public HashSet<string> z = new HashSet<string>();    // Алфавит магазинных сомволов  
            static public string v = "";                                // Выходной  алфавит

            static Config() {
                Q = new Dictionary<Condition, Rule>();
                Z = new Stack<string>();
                Update();
            }

            public Config(string x, string y, string z) {
                c = new Condition(x, y, z);
            }

            public Config(string[] data) : this(data[0], data[1], data[2]) {
                r = new Rule(data[3], data[4], data[5]);
                q.Add(data[3]);
                V.Add(data[1]);
                z.Add(data[2]);
                v += data[5];
                v = string.Join("", v.Distinct()).Replace("L", "");
            }

            public static void Update() {
                Z.Clear();
                Z.Push("L");
                Z.Push("z");
                i = 0;
            }

            public static void Get_AMP_Info() {
                V.Remove("L");
                Console.WriteLine($"\n P =({{{string.Join(',', q).Trim()}}}, {{{string.Join(',', V).Trim()}}}, {{{string.Join(',', z).Trim()}}}, " +
                                  $"{{{string.Join(',', v.ToCharArray())}}}, Delta, {q_0}, {z_0}, {{{string.Join(',', F).Trim()}}})\n");

                Console.WriteLine(" Delta:");
                foreach(var v in Q) {
                    Console.WriteLine('\t'+v.Key.GetCond() + " = " + v.Value.GetRule());
                }
            }

            public string Info() {
                return $"({c.q}, {(i < W.Length - 1 ? W.Remove(0, i).TrimEnd('L') : 'L')}, " +
                       $"{(Z.Count == 1 ? 'L' : string.Join("", Z).TrimEnd('L'))}, " +
                       $"{(Translat.Length == 1 ? 'L' : Translat.Replace("L", ""))})";
            }

            public void Takt() {

                Translat += Q[this.c].p;

                if (Q[this.c]._g.Length == 2) {     // --------------------- ЕСЛИ ДОБАВЛЯЕМ В СТЕК
                    
                    Z.Push(W[i++].ToString());      // Загоняем в стек обозреваемый символ цепочки

                    this.c.q = Q[this.c]._q;        // Меняем состояние АМП на состояние правила
                    this.c.a = W[i].ToString();     // Двигаем УУ на следующий символ цепочки
                    this.c.t = Z.Peek();            // Обозреваем верхний символ стека
                }

                else if (Q[this.c]._g == "L") {     // --------------------- ЕСЛИ УДАЛЯЕМ ИЗ СТЕКА

                    Z.Pop();                        // Удаляем из стека верхний символ

                    this.c.q = Q[this.c]._q;
                    this.c.a = W[i] == 'L' ? c.a : W[++i].ToString();
                    this.c.t = Z.Peek();
                }
                                                    // ЕСЛИ В СТЕКЕ Z_0 И СОСТОЯНИЕ НЕ МЕНЯЕТСЯ (дочитываем лениту)
                else if (c.q == Q[this.c]._q && c.t == z_0 && Q[c]._g == z_0) {
                    this.c.a = W[i] == 'L' ? c.a : W[++i].ToString();
                }
                                                    // ЛЯМБДА - ТАКТ (состояние УУ изменяется, лента не двигается)
                else if (c.q != Q[this.c]._q && c.t == z_0 && Q[c]._g == z_0) {
                    this.c.q = Q[this.c]._q;
                }

                else {

                }
            }
        }

        static void Reading_Data_from_File() {
            using (StreamReader f = new StreamReader("config.txt")) {
                string str;
                while ((str = f.ReadLine()) != null) {

                    if(!str.Contains('#') && str != "") {
                        if (str.Contains("Начальное состояние"))
                            Config.q_0 = str.Replace("Начальное состояние:", "").Trim();

                        else if (str.Contains("Конечные состояния"))
                            Config.F = str.Replace("Конечные состояния:", "").Split(new char[] { ',', ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries).ToHashSet();

                        else if (str.Contains("Начальный символ"))
                            Config.z_0 = str.Replace("Начальный символ стека:", "").Trim();

                        else {
                            Config tmp = new Config(str.Split(new char[] { '(', ')', ',', '=', ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries));
                            Config.Q.Add(tmp.c, tmp.r);
                        }
                    }
                }
            }

            if (!Config.q.Contains(Config.q_0)) 
                throw new Exception($"Начального состояния: '{Config.q_0}' нет в множестве состояний автомата!");

            foreach(var v in Config.F)
                if (!Config.q.Contains(v))
                    throw new Exception($"Конечного состояния: '{v}' нет в множестве состояний автомата!");

            if (!Config.z.Contains(Config.z_0))
                throw new Exception($"Начального символа: '{Config.z_0}' нет в алфавите магазинных символов!");
        }

        static void Start_AMP() {

            Console.Clear();

            Config.Update();
            Config.Get_AMP_Info();
            Config.Translat = "L";
            Console.Write("\n Введите цепочку: ");
            Config.W = Console.ReadLine().Replace(" ","") + "L";
            Console.WriteLine("\n Результат перевода:");

            Config MO = new Config(Config.q_0, Config.W[0].ToString(), Config.z_0); // Начальное состояние системы

            int Count = 0;
            while (true) {

                if (Count++ % 4 == 0) Console.Write("\n ");

                Console.Write(MO.Info());

                if (!Config.V.Contains(MO.c.a) && MO.c.a != "L") {                  // Если символа нет в алфавите языка
                    Console.WriteLine($"\n\n ERROR: Алфавит не содержит символ '{MO.c.a}', цепочка не принята!");
                    break;
                }

                if (MO.c.a == "L" && MO.c.t == "L") {                               // Если лента и магазин пустые
                    if (Config.F.Contains(MO.c.q))
                        Console.WriteLine("\n\n MESSAGE: Цепочка принята, перевод выполнен успешно");
                    else
                        Console.WriteLine("\n\n ERROR: Лента и магазин пустые, но состояние не конечное, цепочка не принята!");
                    break;
                }

                if (MO.c.a != "L" && MO.c.t == "L") {                               // Если стек пустой, а лента нет
                    Console.WriteLine("\n\n ERROR: Стек пустой, а магазин нет, цепочка не принята!");
                }

                if (!Config.Q.ContainsKey(MO.c)) {                                  // Если нет такого состояния УУ
                    Console.WriteLine($"\n\n ERROR: Нет состояния УУ: {MO.c.GetCond()}, цепочка не принята!");
                    break;
                }

                Console.Write(" -> "); MO.Takt();
            }
            Console.WriteLine();
        }

        static void Main() {

            try {
                Reading_Data_from_File();
            }
            catch (System.IO.FileNotFoundException) {
                Console.WriteLine($"\n ERROR: В корневой директории не найден файл 'config.txt'\n");
                return;
            }
            catch(Exception e) {
                Console.WriteLine($"\n ERROR: {e.Message}\n");
                return;
            }

            while(true) {

                Start_AMP();

                Console.WriteLine(" ПОВТОРИТЬ - Any Key, ЗАВЕРШИТЬ - ESC ?");
                
                if ((int)Console.ReadKey(true).KeyChar == 27)
                    return;
            }
        }
    }
}
