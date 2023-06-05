using System;
using System.Drawing;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace TLP_Lab1 {

    public partial class Form1 : Form {

        class ChainCompare : IComparer<string> {
            public int Compare(string one, string two) {
                return one.Length - two.Length;
            }
        }

        string VT = "01", NT = "STF", Start = "S", symbl = "", rule = "";
        HashSet<string> temp = new HashSet<string>();
        bool GramCond = true, RuleCond = true;
        enum Mode { Left, Right };
        int MIN = 4, MAX = 4;
        Rezult rez;
        Mode mode;

        static List<string> List_NT_Chains = new List<string>();
        static List<string> Rezult = new List<string>();

        Dictionary<string, List<string>> m = new Dictionary<string, List<string>>() 
        {
            {"S", new List<string>{"T"}},
            {"T", new List<string>{"TF", "F"}},
            {"F", new List<string>{"0", "1"}}
        };

        public Form1() {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {

            button2.Enabled = false;
            textBox2.Text = NT;
            textBox3.Text = VT;
            textBox4.Text = Start;
            textBox1.Text = GetStringForView();
            radioButton1.Checked = true;
        }

        //  1) Генерируем цепочки
        private void Get_All_NT_Chain(string rez) {

            Add_NT_Chain(rez);

                switch (mode) {

                    // Левосторонний вывод
                    case Mode.Left:
                        for (int i = 0; i < rez.Length; i++) {
                            if (NT.Contains(rez[i])) {
                                string _V = rez[i].ToString();
                                foreach(var rule in m[_V]) {
                                    if (!VT.Contains(rule)) {
                                        string chain = rez.Remove(i, 1).Insert(i, rule);
                                        if ((rule.Contains(_V) && chain.Length <= MAX + 1) || !rule.Contains(_V))
                                            Get_All_NT_Chain(chain);
                                    }
                                }
                            }
                        }
                        break;

                    // Правосторонний вывод
                    case Mode.Right:
                        for(int i = rez.Length - 1; i >= 0; i--) {
                            if (NT.Contains(rez[i])) {
                                string _V = rez[i].ToString();
                                foreach(var rule in m[_V]) {
                                    if (!VT.Contains(rule)) {
                                        string chain = rez.Remove(i, 1).Insert(i, rule);
                                        if ((rule.Contains(_V) && chain.Length <= MAX + 1) || !rule.Contains(_V))
                                            Get_All_NT_Chain(chain); 
                                    }
                                }
                            }
                        }
                        break;
                }
        }

        //  2) Отбираем цепочки, каждый NV которой порождает терминал
        private void Add_NT_Chain(string chain) {
            foreach (var v in chain)
                if (NT.Contains(v))
                    foreach (var s in m[v.ToString()])
                        if (NT.Contains(s))
                            return;
            List_NT_Chains.Add(chain);
        }

        //  3) Для каждой цепочки из п. 2 генерируем размещения с повторениями VT символов
        private void Get_All_VT_Chain(string rez, bool isFirstView) {

            switch (mode) {
                case Mode.Left:
                    for (int i = 0; i < rez.Length; i++) {
                        if (NT.Contains(rez[i]) && isFirstView) {
                            isFirstView = false;
                            string _V = rez[i].ToString();
                            foreach (var s in m[_V]) {
                                if (VT.Contains(s)) {
                                    rez = rez.Remove(i, 1).Insert(i, s);
                                    if (CheckVN(rez)) {
                                        string chain = rez.Replace("λ", "");
                                        if (chain.Length >= MIN && chain.Length <= MAX)
                                            Rezult.Add(chain);
                                    }
                                    else
                                        Get_All_VT_Chain(rez, true);
                                }
                            }
                        }
                    }
                    break;
                case Mode.Right:
                    for (int i = rez.Length - 1; i >= 0; i--) {
                        if (NT.Contains(rez[i]) && isFirstView) {
                            isFirstView = false;
                            string _V = rez[i].ToString();
                            foreach (var s in m[_V]) {
                                if (VT.Contains(s)) {
                                    rez = rez.Remove(i, 1).Insert(i, s);
                                    if (CheckVN(rez)) {
                                        string chain = rez.Replace("λ", "");
                                        if (chain.Length >= MIN && chain.Length <= MAX)
                                            Rezult.Add(chain);
                                    }
                                    else
                                        Get_All_VT_Chain(rez, true);
                                }
                            }
                        }
                    }
                    break;
            }
        }

        public bool CheckVN(string chain) {
            foreach (var symbl in NT)
                if (chain.Contains(symbl))
                    return false;
            return true;
        }

        // Обработка нажатий кнопок
        private void button_Click(object sender, EventArgs e) {

            switch (((Button)sender).Tag.ToString()) {

                case "Generator":

                        if (Application.OpenForms.Count == 2) {
                            List_NT_Chains.Clear();
                            Rezult.Clear();
                            rez.Close();
                        }

                        rez = new Rezult(this);
                        rez.Show();

                        Get_All_NT_Chain(Start);

                        List_NT_Chains = List_NT_Chains.Distinct().ToList();
                        foreach (var v in List_NT_Chains) 
                            Get_All_VT_Chain(v, true);
                        //Rezult.Sort(new ChainCompare());
                        Rezult = Rezult.Distinct().ToList();

                        if (Rezult.Count == 0)
                            rez.listBox1.Items.Add($"\tEmpty List");
                        else
                            foreach(var v in Rezult)
                                rez.listBox1.Items.Add($"   {v}");

                    break;
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e) {
            MIN = (int)numericUpDown1.Value;
        }

        private void numericUpDown2_ValueChanged(object sender, EventArgs e) {
            MAX = (int)numericUpDown2.Value;
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e) {
            mode = Mode.Left;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e) {
            mode = Mode.Right;
        }

        // Формируем строку и правила грамматики
        private string GetStringForView() {

            string rez = $"{Environment.NewLine}  G({'{'}{string.Join(",", VT.ToArray())}{'}'}, " +
                       $"{'{'}{string.Join(",", NT.ToArray())}{'}'}, P, {'{'}{Start}{'}'})";
            
            foreach(var v in m) 
                rez += $"{Environment.NewLine}  {v.Key} -> {string.Join("|", v.Value)}";

            return rez;
        }

        // Проверка конфигурации грамматики
        private void CheckConfigGrammar() {

            textBox1.Text = Environment.NewLine + "\t\tEmpty";

            if (VT != "" && NT != "" && Start != ""){

                GramCond = true;
                button2.Enabled = m.Count == NT.Length ? true : false;

                textBox1.Text = GetStringForView();
            }

            else {
                GramCond = RuleCond = false;
                textBox5.Text = textBox6.Text = "";
                button2.Enabled = false;
            }

        }

        // Проверка конфигурации правила
        private void CheckConfigRule() {

            symbl = textBox5.Text; rule = textBox6.Text;

            label7.Text = textBox6.Text.Contains("||") || textBox6.Text.Contains("λλ") ? "Bad rule" : "";

            if (GramCond == true && symbl != "" && label7.Text == "" && !textBox1.Text.Contains("Empty")) {

                rule = rule.Trim('|');

                if (!m.ContainsKey(symbl)) {
                    m.Add(symbl, new List<string>());
                }

                if (rule == "") 
                    m.Remove(symbl);
                else {
                    m[symbl].Clear();
                    foreach (var v in rule.Split('|')) m[symbl].Add(v);
                }
                textBox1.Text = GetStringForView();
                RuleCond = true;
            }
            else {
                RuleCond = false;
                button2.Enabled = false;
            }

            button2.Enabled = m.Count == NT.Length && GramCond ? true : false;

        }

        // Правила ввода правил грамматики
        private void Rule_TextChanged(object sender, EventArgs e) {

            switch (((TextBox)sender).Tag.ToString()) {

                case "NT":
                    if (textBox5.Text != "" && NT.Contains(textBox5.Text[0].ToString().ToUpper())) {

                        textBox5.Text = textBox5.Text[0].ToString().ToUpper();
                    }
                    else
                        textBox5.Text = "";

                    if (NT != textBox5.Text) textBox6.Text = "";

                    textBox5.SelectionStart = 0;
                    CheckConfigRule();
                    break;

                case "Rule":
                    string rules = textBox6.Text;

                    foreach(var v in rules) {
                        if (!(VT + NT + "| " + (char)0x3BB).Contains(v)) {
                            rules = rules.Remove(rules.IndexOf(v), 1);
                        }
                    }
                    rules = rules.Replace(' ', (char)0x3BB);

                    textBox6.SelectionStart = textBox6.Text.Length;
                    textBox6.Text = rules;
                    CheckConfigRule();
                    break;
            }
        }

        // Правила ввода грамматики
        private void Grammar_TextChanged(object sender, EventArgs e) {

            switch (((TextBox)sender).Tag.ToString()) {

                case "VT":
                    foreach (var v in textBox3.Text)
                        if (!"|".Contains(v)) {
                            char ch = v == ' ' ? 'λ' : v;
                            temp.Add(ch.ToString().ToLower());
                        }

                    // Если количество VT изменилось - удаляем правила
                    if (VT.CompareTo(string.Join("", temp.ToArray())) != 0) {
                        textBox5.Text = textBox6.Text = "";
                        m.Clear();
                    }

                    VT = textBox3.Text = string.Join("", temp.ToArray());

                    textBox3.SelectionStart = textBox3.Text.Length;
                    break;

                case "NT":
                    foreach (var v in textBox2.Text)
                        if (v >= 'a' && v <= 'z' || v >= 'A' && v <= 'Z')
                            temp.Add(v.ToString().ToUpper());

                    // Если количество NT изменилось - удаляем правила
                    if (NT.CompareTo(string.Join("", temp.ToArray())) != 0) {
                        textBox5.Text = textBox6.Text = "";
                        m.Clear();
                    }

                    NT = textBox2.Text = string.Join("", temp.ToArray());

                    if (!NT.Contains(Start)) Start = textBox4.Text = "";

                    textBox2.SelectionStart = textBox2.Text.Length;
                    break;

                case "S":

                    Start = textBox4.Text;

                    if (Start != "" && NT.Contains(Start[Start.Length-1].ToString().ToUpper())) {

                        Start = textBox4.Text = Start[Start.Length - 1].ToString().ToUpper();
                    }
                    else
                        Start = textBox4.Text = "";

                    textBox4.SelectionStart = textBox4.Text.Length;
                    break;
            }
            temp.Clear();
            CheckConfigGrammar();
        }

        private void Form1_LocationChanged(object sender, EventArgs e) {
            if(Application.OpenForms.Count > 1) {
                Point newLocation = this.Location;
                newLocation.X += this.Width;
                rez.Location = newLocation;
            }
        }
    }
}
