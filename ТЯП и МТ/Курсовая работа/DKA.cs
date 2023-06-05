
using System.Collections.Generic;
using System.Linq;
using System;

namespace TLP_Curs {

    static class DKA {

        public static string[,] Delta;     // Функция переходов
        public static string[] Q;          // Конечное множество состояний УУ автомата
        public static string  A,           // Алфавит автомата
                              q0,          // Начальное состояние автомата
                              Start;       // Начальная цепочка символов

        public static int K;               // Кратность цепочки
        public static HashSet<string> F;   // Множество конечных состояний

        public static void Build_DKA(string a, string s, int k) {
            A = a;
            K = k;
            Start = s;

            // min количество состояний кратное K включая конечное состояние:
            int I = Start.Length + 1 + (K - Start.Length % K);

            // добавленное количество состояний с учетом начальной цепочки:
            int J = (K - 1) - (K - Start.Length % K);

            // тогда всего состояний:
            int CountQ = Start == "" ? K + 1 : I + J;

            // формируем массив всех состояний автомата
            Q = new string[CountQ];
            for (int i = 0; i < Q.Length; i++) Q[i] = "q" + i;

            Delta = new string[Q.Length, A.Length];

            for (int i = 0; i < Q.Length; i++)
                for (int j = 0; j < A.Length; j++)
                    if (i < Start.Length) {
                        Delta[i, j] = A.IndexOf(Start[i]) == j ? Q[i + 1] : "";
                    }
                    else if (i == Q.Length - 1) {
                        Delta[i, j] = Start == "" ? Q[Start.Length + 1] : Q[Start.Length];
                    }
                    else {
                        Delta[i, j] = Q[i + 1];
                    }

            int idx = Start == "" ? K : Start.Length % K == 0 ? Start.Length : I - 1;

            F = new HashSet<string>(1);
            F.Add(Q[idx]);
            q0 = Q[0];
        }

        public static string Check_Chain(string w) {

            int ptr = 0; string Cond = q0, rez = "";

            while (true) {

                if (ptr == w.Length) {
                    rez += F.Contains(Cond) ?  $"({Cond}, {(char)0x3BB}){Environment.NewLine}{Environment.NewLine}Цепочка принята" :
                           !F.Contains(Cond) ? $"({Cond}, {(char)0x3BB}){Environment.NewLine}{Environment.NewLine}Конечное состояние не достигнуто, цепочка не принята!" :
                                               $"{Environment.NewLine}{Environment.NewLine}Неизвестная ошибка!";
                    break;
                }

                if (!A.Contains(w[ptr])) {
                    rez += $"({Cond}, {w.Remove(0, ptr)}){Environment.NewLine}{Environment.NewLine}Алфавит не содержит символ {w[ptr]}, цепочка не принята!";
                    break;
                }
                if (Delta[Array.IndexOf(Q, Cond), A.IndexOf(w[ptr])] == null || Delta[Array.IndexOf(Q, Cond), A.IndexOf(w[ptr])] == "") {
                    rez += $"Empty rule ({Cond}, {w[ptr]}){Environment.NewLine}{Environment.NewLine}Состояние для ({Cond}, {w[ptr]}) не определено!";
                    break;
                }

                rez += $"({Cond}, {w.Remove(0, ptr)}) -> ";

                Cond = Delta[Array.IndexOf(Q, Cond), A.IndexOf(w[ptr])];

                ptr++;
            }

            return rez;
        }

        public static string Get_Config_Info() {

            return $"M=({{{string.Join(",", Q)}}}, {{{string.Join(",", A.ToArray())}}}, " +
                   $"Delta, {{{q0}}}, {{{string.Join(",", F)}}})";
        }

        public static string Get_Delta_Info() {

            string Delta_As_String = $"{Environment.NewLine}Delta:{Environment.NewLine}";

            for (int i = 0; i < Q.Length; i++) {

                Delta_As_String += $"\t({Q[i]},";

                for (int j = 0; j < A.Length; j++) 

                    Delta_As_String += Delta[i, j] == "" ? "" : A[j].ToString();

                Delta_As_String += $") = {Delta[i,A.IndexOf(Delta_As_String[Delta_As_String.Length-1])]}{Environment.NewLine}";
            }

            return Delta_As_String;
        }
    }
}
