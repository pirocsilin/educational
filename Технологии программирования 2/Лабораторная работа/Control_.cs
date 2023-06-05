
namespace Programming_Tech_Form {

    public enum State { editing, converted }

    class Control_ {

        const int pin = 10, pout = 16, acc = 5;
        public History his = new History();
        public Editor ed = new Editor();

        public static State St { get; private set; }
        public int Accuracy { get; set; }
        public int Pin { get; set; }
        public int Pout { get; set; }

        public Control_() {
            St = State.editing;
            Pin = pin;
            Pout = pout;
            Accuracy = acc;
        }

        public string DoCmnd(int KeyNum) {

            // Если нажали Enter
            if (KeyNum == 19) {

                // Конвертируем число
                double r = Convert_p_10.Dval(ed.Number, Pin);
                string res = Convert_10_p.Do(r, Pout, Accuracy);

                St = State.converted;
                his.AddRecord(Pin, Pout, ed.Number, res);

                return res;
            }

            // Если нажата любая другая клавиша
            else {
                St = State.editing;
                return ed.DoEdit(KeyNum);
            }
        }
    }
}
