using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace TLP_Lab2 {
    public partial class Form1 : Form {

        Rezult rez;
        string[,] q;
        Point defoultLoc;
        int defoultHeightForm1;
        bool TableIsExist = false;
        string Q = "", A = "", q0 = "", chain = "";
        HashSet<string> F = new HashSet<string>();
        HashSet<string> temp = new HashSet<string>();

        public Form1() {
            InitializeComponent();
            label7.Visible = true;
            label8.Visible = true;
            button1.Enabled = false;
            defoultHeightForm1 = this.Height;
            defoultLoc = panel1.Location;
        }

        private void UpdateTransFunc() {

            dataGridView1.Columns.Clear();
            dataGridView1.Rows.Clear();
            panel1.Location = defoultLoc;
            this.Height = dataGridView1.Height;
            dataGridView1.Height = 0;
            TableIsExist = false;

            if (Q != "" && A != "") {

                label7.Visible = false;

                for (int i = 0; i < A.Length; i++) {
                    dataGridView1.Columns.Add(A[i].ToString(), A[i].ToString());
                    dataGridView1.Columns[i].SortMode = 0;
                }

                for(int i = 0; i < Q.Length; i++) {
                    dataGridView1.Rows.Add();
                    dataGridView1.Rows[i].HeaderCell.Value = string.Format(Q[i].ToString(), "0");
                }

                q = new string[dataGridView1.RowCount, dataGridView1.ColumnCount]; TableIsExist = true;
                dataGridView1.Height = dataGridView1.RowTemplate.Height * dataGridView1.Rows.Count + dataGridView1.RowTemplate.Height;
                panel1.Location = new Point(panel1.Location.X, panel1.Location.Y + dataGridView1.Height);
                this.Height = defoultHeightForm1 + dataGridView1.Height;
            }
            else {
                label7.Text = "Empty";
                label7.ForeColor = Color.Black;
                label7.Visible = true;
            }
        }

        private void button1_Click(object sender, EventArgs e) {

            if(label7.Visible == false) {

                if (Application.OpenForms.Count == 2) {
                    rez.Close();
                }

                rez = new Rezult(this);
                rez.Show();
                rez.textBox1.Text = f(chain);
            }
        }

        private string f(string w) {

            int ptr = 0; string Cond = q0, rez = "";

            while (true) {

                if (ptr == w.Length) {
                    rez +=  F.Contains(Cond) ? $"({Cond}, {(char)0x3BB}){Environment.NewLine}Цепочка принята" :
                           !F.Contains(Cond) ? $"({Cond}, {(char)0x3BB}){Environment.NewLine}Конечное состояние не достигнуто, цепочка не принята!" :
                                                                     $"{Environment.NewLine}Неизвестная ошибка!";
                    break;
                }

                if (!A.Contains(w[ptr])) {
                    rez += $"({Cond}, {w.Remove(0, ptr)}){Environment.NewLine}Алфавит не содержит символ {w[ptr]}, цепочка не принята!";
                    break;
                }
                if (q[Q.IndexOf(Cond), A.IndexOf(w[ptr])] == null || q[Q.IndexOf(Cond), A.IndexOf(w[ptr])] == "") {
                    rez += $"Empty rule ({Cond}, {w[ptr]}){Environment.NewLine}Состояние для ({Cond}, {w[ptr]}) не определено!";
                    break;
                }

                rez += $"({Cond}, {w.Remove(0, ptr)}) -> ";

                Cond = q[Q.IndexOf(Cond), A.IndexOf(w[ptr])];

                ptr++;
            }

            return rez;
        }

        private void CheckEnableButton() {
            if(Q != "" && A != "" && q0 != "" && F.Count > 0) {
                button1.Enabled = true;
            }
            else
                button1.Enabled = false;
        }

        private void textBox6_TextChanged(object sender, EventArgs e) {

            chain = textBox6.Text = textBox6.Text.Replace(" ", "");

            if (textBox6.Text == "") {
                label8.Visible = true;
                chain = "";
            }
            else {
                textBox6.SelectionStart = textBox6.Text.Length;
                label8.Visible = false;
            }

            CheckEnableButton();
        }

        private void CheckValidData() {

            string ERR = "";

            for (int i = 0; i < dataGridView1.RowCount; i++)
                for (int j = 0; j < dataGridView1.ColumnCount; j++)
                    if (q[i, j] != null && (!Q.Contains(q[i, j]))) {
                        ERR += $" {q[i, j]}";
                    }

            if (ERR != "") {
                label7.Text = "Error in: " + ERR;
                label7.ForeColor = Color.Red;
                label7.Visible = true;
            }
            else {
                label7.Visible = false;
            }

            CheckEnableButton();
        }

        private void dataGridView1_CellValueChanged(object sender, DataGridViewCellEventArgs e) {

            if (TableIsExist) {
                string data = dataGridView1.CurrentCell.Value == null ? "" : dataGridView1.CurrentCell.Value.ToString();
                q[dataGridView1.CurrentCell.RowIndex, dataGridView1.CurrentCell.ColumnIndex] = data;
                CheckValidData();
            }
        }

        private void TextChangedArea(object sender, EventArgs e) {

            switch (((TextBox)sender).Tag.ToString()) {

                case "Q":
                    foreach (var v in textBox1.Text)
                        if (v >= 'a' && v <= 'z' || v >= 'A' && v <= 'Z')
                            temp.Add(v.ToString().ToUpper());

                    if (Q != string.Join("", temp.ToArray())) {
                        Q = textBox1.Text = string.Join("", temp.ToArray());
                        UpdateTransFunc();
                    }
                    else
                        Q = textBox1.Text = string.Join("", temp.ToArray());

                    textBox1.SelectionStart = textBox1.Text.Length;

                    TextChangedArea(textBox3, e);
                    TextChangedArea(textBox4, e);

                    break;

                case "A":
                    foreach (var v in textBox2.Text) {
                        char ch = v == ' ' ? 'λ' : v;
                        temp.Add(ch.ToString().ToLower());
                    }

                    if(A != string.Join("", temp.ToArray())) {
                        A = textBox2.Text = string.Join("", temp.ToArray());
                        UpdateTransFunc();
                    }
                    else
                        A = textBox2.Text = string.Join("", temp.ToArray());

                    textBox2.SelectionStart = textBox2.Text.Length;

                    break;

                case "q0":
                    q0 = textBox3.Text;

                    if (q0 != "" && Q.Contains(q0[q0.Length - 1].ToString().ToUpper())) {

                        q0 = textBox3.Text = q0[q0.Length - 1].ToString().ToUpper();
                    }
                    else
                        q0 = textBox3.Text = "";

                    textBox3.SelectionStart = textBox3.Text.Length;

                    break;

                case "F":
                    F.Clear();

                    foreach(var v in textBox4.Text) {
                        if (Q.Contains(v.ToString().ToUpper()))
                            F.Add(v.ToString().ToUpper());
                    }

                    textBox4.Text = string.Join("", F.ToArray());

                    textBox4.SelectionStart = textBox4.Text.Length;

                    break;
            }

            temp.Clear();
            CheckEnableButton();
        }

        private void Changed_Location_or_Resize(object sender, EventArgs e) {
            if (Application.OpenForms.Count > 1) {
                Point newLocation = this.Location;
                newLocation.Y += this.Height;
                rez.Location = newLocation;
            }
        }
    }
}
