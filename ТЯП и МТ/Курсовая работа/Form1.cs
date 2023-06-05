using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using System.Drawing.Drawing2D;
using System.IO;

namespace TLP_Curs {
    public partial class Form1 : Form {

        Table table;
        Graph graph;
        Rezult rezult;
        bool ConfigIsValid;

        public Form1() {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e) {
            button1.Enabled = false;
            ConfigIsValid = false;
            this.FormBorderStyle = FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
        }

        private bool FormIsOpen(string Name) {

            foreach (var v in Application.OpenForms)
                if (Name == v.GetType().Name.ToString())
                    return true;
            return false;
        }

        private void radioButton_CheckedChanged(object sender, EventArgs e) {

            switch (((RadioButton)sender).Tag.ToString()) {

                case "Table":

                    if (FormIsOpen("Table") && radioButton1.Checked == true) {
                        UpdateTable();
                    }

                    else if (FormIsOpen("Table") && radioButton1.Checked == false) {
                        table.Close();
                    }

                    else if (radioButton1.Checked == true && ConfigIsValid == true) {
                        table = new Table(this);
                        UpdateTable();
                        table.Show();
                        this.Focus();
                    }
                    break;

                case "Graph":

                    if (FormIsOpen("Graph") && radioButton2.Checked == true) {
                        graph.Width = DKA.Q.Length * 80 + 60;
                        UpdateGraph();
                    }

                    else if (FormIsOpen("Graph") && radioButton2.Checked == false) {
                        graph.Close();
                    }

                    else if (radioButton2.Checked == true && ConfigIsValid == true) {
                        graph = new Graph(this);
                        graph.Width = DKA.Q.Length * 80 + 60;
                        graph.Show();
                        UpdateGraph();
                        this.Focus();
                    }
                    break;
            }
        }

        private void UpdateGraph() {

            int radius = 50;
            graph.Controls.Clear();
            Graphics g = graph.CreateGraphics();
            g.Clear(Color.White);

            Pen LineStyle = new Pen(Color.Black, 1);
            LineStyle.CustomEndCap = new AdjustableArrowCap(5, 5);

            for (int i = 0; i < DKA.Q.Length; i++) {

                // Рисуем вершины графа
                g.DrawEllipse(new Pen(Color.Black), 30 + i*80, 70, radius, radius);

                // Рисуем кривую Безье образуя переход из конечного узла графа в первый Аi-тый узел
                if(i == DKA.Q.Length - 1) {
                    if (DKA.Q[i] == DKA.Delta[i,0]) {

                        int X = 30 + i * 80 + radius;
                        g.DrawBezier(LineStyle, new Point(X, 70 + radius / 2), new Point(X + radius, 70 + 3 * radius / 2),
                                                new Point(X - radius / 3, 70 + 2 * radius), new Point(X - radius / 2, 70 + radius));
                    }
                    else {
                        int Start_X = 30 + i * 80 + radius / 2;
                        int End_X = DKA.Start != "" ? 30 + (DKA.Start.Length) * 80 + radius / 2 :
                                                      30 + (DKA.Start.Length + 1) * 80 + radius / 2;
                        int Delta = Start_X - End_X;

                        g.DrawBezier(LineStyle, new Point(Start_X, 70 + radius), new Point(Start_X - Delta / 3, 70 + 2 * radius),
                                                new Point(Start_X - 2 * Delta / 3, 70 + 2 * radius), new Point(End_X, 70 + radius));
                    }
                }
                
                // Рисуем ребра графа
                if(i < DKA.Q.Length - 1) {
                    g.DrawLine(LineStyle, 30 + i * 80 + radius, 70 + radius / 2, 2 * 30 + i * 80 + radius, 70 + radius / 2);
                }

                // Подписываем ребра
                g.DrawString(i < DKA.Start.Length ? DKA.Start[i].ToString() : "Ai", new Font("Arial", 11), Brushes.Black, 
                    new Point(33 + i * 80 + (i < DKA.Q.Length - 1 ? radius : 2*radius/3), 70 + radius / (i < DKA.Q.Length - 1 ? 14 : 1)));

                // Вписываем состояния в узлы графа
                g.DrawString(DKA.Q[i], new Font("Arial", 13), Brushes.Black, new Point(40 + i * 80, 80));

                // Обозначаем конечное состояние
                if (DKA.F.Contains(DKA.Q[i])) {
                    g.DrawEllipse(new Pen(Color.Black), 35 + i * 80, 75, radius-10, radius-10);
                }
            }
            g.Dispose();
        }

        private void UpdateTable() {

            table.dataGridView1.Columns.Clear();
            table.dataGridView1.Rows.Clear();
            table.dataGridView1.RowHeadersWidth = 60;
            table.dataGridView1.ColumnHeadersDefaultCellStyle.Alignment = DataGridViewContentAlignment.MiddleCenter;

            for (int i = 0; i < DKA.A.Length; i++) {
                table.dataGridView1.Columns.Add(DKA.A[i].ToString(), DKA.A[i].ToString());
                table.dataGridView1.Columns[i].SortMode = 0;
            }

            for (int i = 0; i < DKA.Q.Length; i++) {
                 table.dataGridView1.Rows.Add();
                 table.dataGridView1.Rows[i].HeaderCell.Value = string.Format(DKA.Q[i].ToString(), "0");
            }

            for (int i = 0; i < table.dataGridView1.RowCount; i++)
                for (int j = 0; j < table.dataGridView1.ColumnCount; j++)
                    table.dataGridView1.Rows[i].Cells[j].Value = DKA.Delta[i, j];

            table.dataGridView1.ClearSelection();
            table.Height = table.dataGridView1.RowTemplate.Height * table.dataGridView1.Rows.Count + 3 * table.dataGridView1.RowTemplate.Height;
        }

        private void textBox_TextChanged(object sender, EventArgs e) {

            string tmp = "";

            switch (((TextBox)sender).Tag) {
                case "ABC":
                    HashSet<char> temp = new HashSet<char>();
                    foreach (var v in textBox1.Text)
                        if(v >= 'a' && v <= 'z' || v >= '0' && v <= '9')
                            temp.Add(v);

                    textBox1.Text = string.Join("", temp);
                    textBox1.SelectionStart = textBox1.Text.Length;

                    textBox_TextChanged(textBox2, e);
                    Check_Config();
                    break;

                case "SChain":
                    foreach (var v in textBox2.Text)
                        if (textBox1.Text.Contains(v.ToString()))
                            tmp += v;

                    textBox2.Text = tmp;
                    textBox2.SelectionStart = textBox2.Text.Length;
                    Check_Config();
                    break;

                case "UserChain":
                    foreach (var v in textBox4.Text)
                        if (v >= 'a' && v <= 'z' || v >= '0' && v <= '9')
                            tmp += v;

                    textBox4.Text = tmp;
                    textBox4.SelectionStart = textBox4.Text.Length;
                    button1.Enabled = (textBox1.Text != "" && textBox4.Text != "") ? true : false;
                    label6.Text = textBox4.Text != "" ? "" : "Empty chain!";
                    break;
            }
            if (FormIsOpen("Rezult")) rezult.Close();
        }

        private void Check_Config() {

            if (FormIsOpen("Rezult")) rezult.Close();

            if (textBox1.Text != "") {
                ConfigIsValid = true;
                DKA.Build_DKA(textBox1.Text, textBox2.Text, (int)numericUpDown1.Value);

                textBox3.Text = DKA.Get_Config_Info();

                RadioButton OpenForm = radioButton1.Checked == true ? radioButton1 : radioButton2;

                radioButton_CheckedChanged(OpenForm, new EventArgs());
            }
            else {
                ConfigIsValid = false;

                textBox3.Text = "Empty DKA";

                if (FormIsOpen("Table")) table.Close();
                if (FormIsOpen("Graph")) graph.Close();
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e) => Check_Config();

        private void button1_Click(object sender, EventArgs e) {

            if (FormIsOpen("Rezult")) {
                rezult.Close();
            }

            rezult = new Rezult(this);
            rezult.Show();

            string Result = DKA.Check_Chain(textBox2.Text + textBox4.Text);
            rezult.textBox1.Text = Result;

            if (checkBox1.Checked) {
                using (StreamWriter writer = new StreamWriter($"Last_Checked_Result.txt", false)) {
                    writer.WriteLine(DKA.Get_Config_Info());
                    writer.WriteLine(DKA.Get_Delta_Info());
                    writer.WriteLine(Result);
                }

            }
        }

        private void Form1_LocationChanged(object sender, EventArgs e) {

            Point newLocation = this.Location;
            newLocation.X += this.Width;

            if (FormIsOpen("Table")) table.Location = newLocation;

            if (FormIsOpen("Graph")) graph.Location = newLocation;

            if (FormIsOpen("Rezult")) {
                newLocation = this.Location;
                newLocation.Y += this.Height;
                rezult.Location = newLocation;
            }
        }

        private void button2_Click(object sender, EventArgs e) {
            MessageBox.Show(
                "\tАвтор:    Трюшников Вадим Георгиевич\n" +
                "\tГруппа:   ПБВ-03\n" +
                "\tВариант:  №1\n\n" +
                "\tТема курсового проекта: \n\n\t<< ДЕТЕРМИНИРОВАННЫЙ КОНЕЧНЫЙ АВТОМАТ >>",
                "Информация о проекте",
                MessageBoxButtons.OK,
                MessageBoxIcon.None,
                MessageBoxDefaultButton.Button1
                );
        }
    }
}
