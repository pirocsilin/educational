using System;

namespace Programming_Tech_Form {

    class Editor {
        //Разделитель целой и дробной частей.
        const string delim = ",";
        //Ноль.
        const string zero = "0";
        // Число
        private string number = zero;
        //Свойствое для чтения редактируемого числа
        public string Number {
            get { return number; }
        }

        //Добавить цифру.
        public void AddDigit(char key) {
            foreach (var ch in "0123456789AaBbCcDdEeFf") {
                if (key == ch) {
                    if (number == zero) Clear();
                    number += ch.ToString().ToUpper();
                }
            }
        }

        // Добавить разделитель
        public void AddDelim() {
            bool DelimInNum = false;

            foreach (var ch in number)
                if (ch.ToString() == delim)
                    DelimInNum = true;

            if (!DelimInNum) number += delim;
        }

        // Удалить символ справа
        public void Bs() {
            if (number != "")
                number = number.Remove(number.Length - 1);
            if (number == "")
                number = zero;
        }

        // Очистить редактируемое число
        public void Clear() { number = ""; }

        // Выполнить команду редактирования
        public string DoEdit(int Key) {

            switch (Key) {

                // Добавить запятую
                case 16:
                    AddDelim(); break;

                // Удалить символ
                case 17:
                    Bs(); break;

                // Сбросить число на ноль
                case 18:
                    number = zero; break;
                
                // Добавить цифру к числу
                default:
                    AddDigit((char)Key); break;
            }

            return Number;
        }
    }
}
