using System;
using System.Drawing;
using System.Windows.Forms;

namespace Programming_Tech_Form {

    public partial class Form1 : Form {

        Control_ ctrl = new Control_();
        AboutBox about;
        HistoryBox history;

        public Form1() {
            InitializeComponent();
            this.KeyPreview = true;
        }

        // Загрузке формы
        private void Form1_Load(object sender, EventArgs e) {

            label1.Text = ctrl.ed.Number;
            trackBar1.Value = ctrl.Pin;
            trackBar2.Value = ctrl.Pout;
            numericUpDown1.Value = ctrl.Pin;
            numericUpDown2.Value = ctrl.Pout;
            label3.Text = "Основание с. сч. исходного числа " + trackBar1.Value;
            label4.Text = "Основание с. сч. результата " + trackBar2.Value;
            label2.Text = "0";
            UpdateButtons();
        }

        // Изменяем состояние командных кнопок
        private void UpdateButtons() {

            foreach (Control ctr in flowLayoutPanel1.Controls) {

                int j = Convert.ToInt32(ctr.Tag.ToString());

                ctr.Enabled = (j >= trackBar1.Value && j <= 15) ? false : true;
            }
        }

        private void DoCmnd(int Key) {

            if (Key == 19) {
                try {
                    label2.Text = ctrl.DoCmnd(19);
                }
                catch (OverflowException) {
                    label3.Text = "ПЕРЕПОНЕНИЕ. Уменьшите число!";
                }
            }
            else {
                if (Control_.St == State.converted) {
                    label1.Text = ctrl.DoCmnd(18);
                }
                label1.Text = ctrl.DoCmnd(Key);
                label2.Text = "0";
            }
        }

        // Обработка нажатия командных кнопок 
        private void button_Click(object sender, EventArgs e) {

            int Key = Convert.ToInt32(((Button)sender).Tag.ToString());

            Key = e.GetType().Name == "EventArgs" ? 19  :
                  (Key >= 16 && Key <= 19)        ? Key :
                  Convert.ToChar(((Button)sender).Text);

            DoCmnd(Key);
        }

        // Обработка клавиш управления
        private void Form1_KeyDown(object sender, KeyEventArgs e) {

            if(e.KeyCode == Keys.Delete)    DoCmnd(18);    // entf
            if (e.KeyCode == Keys.Execute)  DoCmnd(19);    // enter
        }

        // Обработка алфавитно-цифровых клавиш
        private void Form1_KeyPress(object sender, KeyPressEventArgs e) {

            int i  = e.KeyChar >= 'A' && e.KeyChar <= 'F' ? (int)e.KeyChar - 'A' + 10 :
                     e.KeyChar >= 'a' && e.KeyChar <= 'f' ? (int)e.KeyChar - 'a' + 10 :
                     e.KeyChar >= '0' && e.KeyChar <= '9' ? (int)e.KeyChar - '0'      :

                     e.KeyChar == '.' || e.KeyChar == ',' ? 16 :
                     (int)e.KeyChar == 8                  ? 17 : -1;

            if(i >= 0 && i < ctrl.Pin)  DoCmnd((int)e.KeyChar);

            if(i == 16 || i == 17)      DoCmnd(i);
        }

        // Каждое перемещение ползунка trackBar1
        private void trackBar1_Scroll(object sender, EventArgs e) {
            numericUpDown1.Value = trackBar1.Value;
            this.UpdateP1();
        }

        // При изменении значения numericUpDown1
        private void numericUpDown1_ValueChanged(object sender, EventArgs e) {
            trackBar1.Value = Convert.ToInt32(numericUpDown1.Value);
            this.UpdateP1();
        }

        // Обновляем интерфейс связанный с основанием исходного числа
        private void UpdateP1() {
            label3.Text = "Основание с. сч. исходного числа " + trackBar1.Value;
            ctrl.Pin = trackBar1.Value;
            UpdateButtons();
            label1.Text = ctrl.DoCmnd(18);
            label2.Text = "0";
        }

        // Каждое перемещение ползунка trackBar2
        private void trackBar2_Scroll(object sender, EventArgs e) {
            numericUpDown2.Value = trackBar2.Value;
            UpdateP2();

        }

        // При изменении значения numericUpDown2
        private void numericUpDown2_ValueChanged(object sender, EventArgs e) {
            trackBar2.Value = Convert.ToInt32(numericUpDown2.Value);
            UpdateP2();
            DoCmnd(19);
        }

        // Обновляем интерфейс связанный с основанием результата
        private void UpdateP2() {
            label4.Text = "Основание с. сч. результата " + trackBar2.Value;
            ctrl.Pout = trackBar2.Value;
        }

        // Обработка кнопок меню 
        private void toolStripMenuItems_Click(object sender, EventArgs e) {

            ToolStripMenuItem Item = (ToolStripMenuItem)sender;
            bool CheckActiveForm = false;
            Point newPosition = this.Location;
            newPosition.X += this.Width;

            switch (Item.Tag.ToString()) {

                case "Exit":
                    this.Close();
                    break;

                case "History":

                    if(about != null) about.Close();

                    foreach (Form form in Application.OpenForms) {
                        if (form.Name == "HistoryBox")
                            CheckActiveForm = true;
                    }
                    if (!CheckActiveForm) {
                        history = new HistoryBox(this);
                        history.Location = newPosition;
                        if(History.Count == 0) {
                            history.listBox1.Items.Add("История пуста!");
                        }
                        else {
                            for(int i = 0; i < History.Count; i++) {
                                history.listBox1.Items.Add(ctrl.his[i]);
                            }
                        }
                        history.Show();
                    }
                    break;

                case "About":

                    if(history != null) history.Close();

                    foreach (Form form in Application.OpenForms) {
                        if (form.Name == "AboutBox")
                            CheckActiveForm = true;
                    }
                    if (!CheckActiveForm) {
                        about = new AboutBox(this);
                        about.Location = newPosition;
                        about.Show();
                    }
                    break;
            }
        }

        // Позиция второго активного окна приложения
        private void Form1_LocationChanged(object sender, EventArgs e) {

            Point newPosition = this.Location;
            newPosition.X += this.Width;

            foreach (Form form in Application.OpenForms) {

                if (form.Name == "AboutBox") {
                    about.Location = newPosition;
                }

                if (form.Name == "HistoryBox") {
                    history.Location = newPosition;
                }
            }
        }
    }
}
