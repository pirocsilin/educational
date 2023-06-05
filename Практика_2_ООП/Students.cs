
using System;
using System.Collections.Generic;

namespace Students_List_Form {

    abstract class Students {

        private string fio;
        private string prevEduc;
        private int yearOfBirth;

        public string PrevEduc { 
            set { prevEduc = value.Trim(); } 
            get { return prevEduc; }
        }

        public string FIO {         // управление доступом к переменной fio
            set {
                string[] arr = value.Trim().Split(new char[] { ' ' },
                                StringSplitOptions.RemoveEmptyEntries);

                if (arr.Length == 3) {
                    fio = string.Join(" ", arr);
                }
                else {
                    throw new Exception("ФИО должно быть из трех слов");
                }
            }
            get { return fio; }
        }

        public int YearOfBirth {    // управление доступом к переменной yearOfBirth
            set {
                if (value >= 1920 && value <= 2005) {
                    yearOfBirth = value;
                }
                else {
                    throw new Exception("min 1920, max 2005");
                }
            }
            get { return yearOfBirth; }
        }

        // Вызов конструктора по умолчанию инициализирует
        // поля обьекта значениями по умолчанию
        public Students() { }

        public Students(string fio, int yearOfBirth, string prevEduc) {
            FIO = fio;
            YearOfBirth = yearOfBirth;
            PrevEduc = prevEduc;
        }

        public void Info() => Console.WriteLine($" {fio}:\t{yearOfBirth}\t{prevEduc}");
    }

    class Budget : Students {

        public string Faculty { get; set; }     // факультет
        public string Direction { get; set; }   // Направление
        public string Group { get; set; }       // Группа

        public static List<Budget> List = new List<Budget>();

        public Budget() { }

        public Budget(string fio, int yearOfBirth, string prevEduc, string faculty,
                      string direction, string group) : base(fio, yearOfBirth, prevEduc) {

            Faculty = faculty;
            Direction = direction;
            Group = group;
        }

        public virtual string FormatData() => $"{FIO}\t{YearOfBirth}\t{PrevEduc}" +
                                              $"\t{Faculty}\t{Direction}\t{Group}";
    }

    class Paid : Budget {

        public static new List<Paid> List = new List<Paid>();

        public Paid() { }

        public Paid(string fio, int yearOfBirth, string prevEduc, string faculty,
                      string direction, string group) :
            base(fio, yearOfBirth, prevEduc, faculty, direction, group) { }

        public override string FormatData() => "Paid student:\n" + base.FormatData();
    }

    static class Education {

        // Образование
        public static List<string> StartEducation = new List<string> {
            "Среднее общее (11 классов)",
            "Среднее профессиональное",
            "Высшее (бакалавриат и выше)"
        };

        // Факультет
        public static List<string> Faculty = new List<string> {
            "Автоматической электросвязи",
            "Мультисервисных телекоммуникационных систем",
            "Мобильной радиосвязи и мультимедиа",
            "Информатики и вычислительной техники",
            "Инженерно-экономический факультет"
        };

        // Направление
        public static List<string> Direction = new List<string> {
            "01.03.02 | Прикладная математика и информатика",
            "02.03.02 | Фундаментальные информатика и информационные технологии",
            "09.03.01 | Информатика и вычислительная техника",
            "09.03.02 | Информационные системы и технологии",
            "09.03.03 | Прикладная информатика",
            "10.03.01 | Информационная безопасность",
            "10.05.02 | Информационная безопасность телекоммуникационных систем",
            "11.03.01 | Радиотехника",
            "11.03.02 | Инфокоммуникационные технологии и системы связи",
            "11.03.03 | Конструирование и технология электронных средств",
            "11.03.04 | Электроника и наноэлектроника",
            "11.05.01 | Радиоэлектронные системы и комплексы",
            "11.05.04 | Инфокоммуникационные технологии и системы специальной связи"
        };

        // Бюджетные группы
        public static List<string> BudgetGoups = new List<string> {
            "БСТ-1",    "БСТ-2",    "БСТ-3",
            "ТЭС-12",   "ТЭС-13",   "ТЭС-14",
            "ПМИ-22",   "ПМИ-23",   "ПМИ-24",
            "ИНФ-4",    "ИНФ-5",    "ИНФ-6"
        };

        // Бюджетные группы
        public static List<string> PaidGoups = new List<string> {
            "БСТ-41",    "БСТ-42",    "БСТ-43",
            "ТЭС-52",   "ТЭС-53",   "ТЭС-54",
            "ПМИ-62",   "ПМИ-63",   "ПМИ-64",
            "ИНФ-44",    "ИНФ-45",    "ИНФ-46"
        };
    }
}
