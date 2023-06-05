using System;
using System.Collections.Generic;

public struct Record {

    int p1;
    int p2;
    string number1;
    string number2;

    public Record(int p1, int p2, string n1, string n2) {
        this.p1 = p1;
        this.p2 = p2;
        number1 = n1;
        number2 = n2;
    }
    public override string ToString() {
        return p1.ToString() + ": " + number1 + " => " + p2.ToString() + ": " + number2;
    }
}

namespace Programming_Tech_Form {

    public class History {

        public static int Count { get; private set; }
        private static List<Record> L;

        // Статический конструктор вызывается один раз при первом обращении к
        // static члену или создании объекта класса. В данном случае используется
        // для инициализации списка структур Record и установки значения Count
        static History() {
            L = new List<Record>();
            Count = 0;
        }

        // Индексатор для получения элемента по индексу
        public Record this[int i] {
            get {
                if (Count == 0)
                    throw new Exception("Список пуст!");
                if (i >= Count || i < 0)
                    throw new Exception("Индекс вне диапазона списка!");

                return L[i];
            }
        }

        // Добавить запись в список
        public void AddRecord(int p1, int p2, string n1, string n2) {
            L.Add(new Record(p1, p2, n1, n2));
            Count += 1;
        }

        // Очистка списка
        public void Clear() {
            L.Clear();
            Count = 0;
        }
    }
}
