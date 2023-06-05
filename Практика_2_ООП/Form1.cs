using System;
using System.Data.SQLite;
using System.Windows.Forms;
using System.Drawing;

namespace Students_List_Form {

    enum Kind { BudgetTable, PaidTable }

    public partial class Form1 : Form {

        private const string DBname = "Students.db";
        private static Kind CurrentTable;

        public Form1() {
            InitializeComponent();
        }

        private void DownloadTable(string TableName) {

            button_Add_To_List.Enabled = true;
            button_Del_From_List.Enabled = true;
            label_Error_FIO.Text = "";
            label_Error_Year.Text = "";
            label_Hint_Del_From_List.Text = "[Выберите Ф.И.О. для удаления]";

            // Подключаемся к базе данных (создаем БД если не существует)
            var connection = new SQLiteConnection($"Data Source={DBname}");
            connection.Open();

            // Создаем объект для выполнения запросов к БД
            SQLiteCommand msg = new SQLiteCommand(connection);

            // Установка команды SQL и её выполнение
            msg.CommandText = DescriptionTable(TableName);
            msg.ExecuteNonQuery();

            // Установка запроса для получения всех данных из таблицы
            msg.CommandText = $"SELECT * FROM {TableName}";

            // Создаем объект для чтения из базы данных
            SQLiteDataReader reader = msg.ExecuteReader();

            // Добавлям данные из базы в соответствующий список
            switch (CurrentTable) {

                case Kind.BudgetTable:
                    button_LoadBudgetList.Enabled = false;
                    button_LoadPaidList.Enabled = true;
                    listView1.Items.Clear();
                    Budget.List.Clear();

                    // Проверяем наличие данных в БД
                    if (!reader.HasRows) { label_HEADER.Text = "СПИСОК БЮДЖЕТНИКОВ ПУСТ"; break; }

                    int n = 1;
                    label_HEADER.Text = "ЗАГРУЖЕН СПИСОК БЮДЖЕТНИКОВ";

                    while (reader.Read()) {
                        Budget.List.Add(new Budget(reader.GetValue(1).ToString(), Convert.ToInt32(reader.GetValue(2).ToString()),
                                        reader.GetValue(3).ToString(), reader.GetValue(4).ToString(), reader.GetValue(5).ToString(),
                                        reader.GetValue(6).ToString()));
                    }

                    foreach (Budget val in Budget.List) listView1.Items.Add(new ListViewItem(new string[] {
                             n++ + ". " + val.FIO, val.YearOfBirth.ToString(), val.PrevEduc, val.Faculty, val.Direction, val.Group }));
                    break;

                case Kind.PaidTable:
                    button_LoadPaidList.Enabled = false;
                    button_LoadBudgetList.Enabled = true;
                    listView1.Items.Clear();
                    Paid.List.Clear();

                    if (!reader.HasRows) { label_HEADER.Text = "СПИСОК ПЛАТНИКОВ ПУСТ"; break; }

                    n = 1;
                    label_HEADER.Text = "ЗАГРУЖЕН СПИСОК ПЛАТНИКОВ";

                    while (reader.Read()) {
                        Paid.List.Add(new Paid(reader.GetValue(1).ToString(), Convert.ToInt32(reader.GetValue(2).ToString()),
                                        reader.GetValue(3).ToString(), reader.GetValue(4).ToString(), reader.GetValue(5).ToString(),
                                        reader.GetValue(6).ToString()));
                    }

                    foreach (Paid val in Paid.List) listView1.Items.Add(new ListViewItem(new string[] {
                             n++ + ". " + val.FIO, val.YearOfBirth.ToString(), val.PrevEduc, val.Faculty, val.Direction, val.Group }));
                    break;
            }

            connection.Close();

            // Загружаем списки видов Образований, Направлений, Факультетов, Групп:
            comboBoxGroup.Items.Clear();
            switch (CurrentTable) {
                case Kind.BudgetTable:
                    foreach (string val in Education.BudgetGoups) comboBoxGroup.Items.Add(val);
                    break;
                case Kind.PaidTable:
                    foreach (string val in Education.PaidGoups) comboBoxGroup.Items.Add(val);
                    break;
            }
            comboBoxGroup.SelectedItem = comboBoxGroup.Items[0].ToString();

            if (comboBoxStartEducation.SelectedIndex == -1) {
                comboBoxStartEducation.Items.AddRange(Education.StartEducation.ToArray());
                comboBoxStartEducation.SelectedItem = comboBoxStartEducation.Items[0];

                comboBoxDirection_.Items.AddRange(Education.Direction.ToArray());
                comboBoxDirection_.SelectedItem = comboBoxDirection_.Items[0].ToString();

                comboBoxFaculty.Items.AddRange(Education.Faculty.ToArray());
                comboBoxFaculty.SelectedItem = comboBoxFaculty.Items[0].ToString();
            }
        }

        // Загружаем таблицу Бюджетников
        private void buttonLoadBudgetList_Click(object sender, EventArgs e) {

            if (buttonSaveData.Enabled == true) ShowDialogSaveOrDecly();

            CurrentTable = Kind.BudgetTable;
            DownloadTable("Budget");
        }

        // Загружаем таблицу Контрактников
        private void buttonLoadPaidList_Click(object sender, EventArgs e) {

            if (buttonSaveData.Enabled == true) ShowDialogSaveOrDecly();

            CurrentTable = Kind.PaidTable;
            DownloadTable("Paid");
        }

        public string DescriptionTable(string Name) {

            return $"CREATE TABLE IF NOT EXISTS {Name} (" +
                    "ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL," +
                    "FIO TEXT NOT NULL, Year_Of_Birth INTEGER NOT NULL," +
                    "Prev_Educ TEXT NOT NULL, Faculty TEXT NOT NULL," +
                    "Direction TEXT NOT NULL, _Group TEXT NOT NULL);";
        }

        // Добавить запись в список
        private void button_Add_To_List_Click(object sender, EventArgs e) {

            if(CurrentTable == Kind.BudgetTable) {

                Budget studentOnBudget = new Budget();

                try {
                    studentOnBudget.FIO = textBoxFIO.Text;
                }
                catch (Exception err) {
                    label_Error_FIO.Text = err.Message;
                }

                try {
                    studentOnBudget.YearOfBirth = Convert.ToInt32(textBoxYearOfByrth.Text);
                }
                catch (FormatException) {
                    label_Error_Year.Text = "Должно быть число";
                }
                catch (Exception err) {
                    label_Error_Year.Text = err.Message;
                }

                if (studentOnBudget.FIO != null && studentOnBudget.YearOfBirth != 0) {
                    studentOnBudget.PrevEduc = comboBoxStartEducation.Text;
                    studentOnBudget.Direction = comboBoxDirection_.Text;
                    studentOnBudget.Faculty = comboBoxFaculty.Text;
                    studentOnBudget.Group = comboBoxGroup.Text;
                    Budget.List.Add(studentOnBudget);

                    listView1.Items.Add(new ListViewItem(new string[] {
                        Budget.List.Count + ". " + studentOnBudget.FIO, studentOnBudget.YearOfBirth.ToString(),
                        studentOnBudget.PrevEduc, studentOnBudget.Faculty, studentOnBudget.Direction, studentOnBudget.Group }));

                    // Активируем кнопку "СОХРАНИТЬ"
                    buttonSaveData.Enabled = true;
                    buttonSaveData.BackColor = Color.FromArgb(255, 128, 128);
                }
            }
            if (CurrentTable == Kind.PaidTable) {

                Paid studentOnPaid = new Paid();

                try {
                    studentOnPaid.FIO = textBoxFIO.Text;
                }
                catch (Exception err) {
                    label_Error_FIO.Text = err.Message;
                }

                try {
                    studentOnPaid.YearOfBirth = Convert.ToInt32(textBoxYearOfByrth.Text);
                }
                catch (FormatException) {
                    label_Error_Year.Text = "Должно быть число";
                }
                catch (Exception err) {
                    label_Error_Year.Text = err.Message;
                }

                if (studentOnPaid.FIO != null && studentOnPaid.YearOfBirth != 0) {
                    studentOnPaid.PrevEduc = comboBoxStartEducation.Text;
                    studentOnPaid.Direction = comboBoxDirection_.Text;
                    studentOnPaid.Faculty = comboBoxFaculty.Text;
                    studentOnPaid.Group = comboBoxGroup.Text;
                    Paid.List.Add(studentOnPaid);

                    listView1.Items.Add(new ListViewItem(new string[] {
                        Paid.List.Count + ". " + studentOnPaid.FIO, studentOnPaid.YearOfBirth.ToString(),
                        studentOnPaid.PrevEduc, studentOnPaid.Faculty, studentOnPaid.Direction, studentOnPaid.Group }));

                    buttonSaveData.Enabled = true;
                    buttonSaveData.BackColor = Color.FromArgb(255, 128, 128);
                }
            }
        }

        // Удаляем выбранный элемент
        private void button_Del_From_List_Click(object sender, EventArgs e) {

            if (listView1.SelectedIndices.Count != 0) {

                int CurrIndex = listView1.SelectedIndices[0];
                listView1.SelectedItems.Clear();

                int n = 1;
                listView1.Items.Clear();
                // Заново загружаем список на listView для восстановления нумерации
                switch (CurrentTable) {
                    case Kind.BudgetTable:
                        Budget.List.RemoveAt(CurrIndex);

                        foreach (Budget val in Budget.List) listView1.Items.Add(new ListViewItem(new string[] {
                                 n++ + ". " + val.FIO, val.YearOfBirth.ToString(), val.PrevEduc, val.Faculty, val.Direction, val.Group }));
                        break;

                    case Kind.PaidTable:
                        Paid.List.RemoveAt(CurrIndex);

                        foreach (Paid val in Paid.List) listView1.Items.Add(new ListViewItem(new string[] {
                                 n++ + ". " + val.FIO, val.YearOfBirth.ToString(), val.PrevEduc, val.Faculty, val.Direction, val.Group }));
                        break;
                }

                // Активируем кнопку "СОХРАНИТЬ"
                buttonSaveData.Enabled = true;
                buttonSaveData.BackColor = Color.FromArgb(255, 128, 128);
            }
        }

        private void listView1_SelectedIndexChanged(object sender, EventArgs e) {

            if (listView1.SelectedIndices.Count != 0) {

                string FIO = listView1.SelectedItems[0].Text;

                label_Hint_Del_From_List.Text = "Удалить: " + FIO + " ?";
            }
            else {
                label_Hint_Del_From_List.Text = "[Выберите Ф.И.О. для удаления]";
            }
        }

        // Сброс ошибки при изменении поля ФИО
        private void textBoxFIO_TextChanged(object sender, EventArgs e) {
            label_Error_FIO.Text = "";
        }

        // Сброс ошибки при изменении поля Дата Рождения
        private void textBoxYearOfByrth_TextChanged(object sender, EventArgs e) {
            label_Error_Year.Text = "";
        }

        // При загрузке формы доступны только кнопки загрузки списков
        private void Form1_Load(object sender, EventArgs e) {
            label_HEADER.Text = "ЗАГРУЗИТЕ СПИСОК БЮДЖЕТНИКОВ ИЛИ ПЛАТНИКОВ";
            button_Add_To_List.Enabled = false;
            button_Del_From_List.Enabled = false;
            buttonSaveData.Enabled = false;
            buttonSaveData.BackColor = Color.FromArgb(255, 192, 192);
        }

        private void ShowDialogSaveOrDecly() {
            DialogResult rezult = MessageBox.Show(
                "Сохранить изменения в таблице?",
                "Message",
                MessageBoxButtons.YesNo,
                MessageBoxIcon.Exclamation,
                MessageBoxDefaultButton.Button1,
                MessageBoxOptions.DefaultDesktopOnly
            );

            if(rezult == DialogResult.Yes) {
                buttonSaveData_Click(rezult, new EventArgs());
            }
            else {
                buttonSaveData.Enabled = false;
                buttonSaveData.BackColor = Color.FromArgb(255, 192, 192);
            }
        }

        // Сохраняем данные в базу данных если есть изменения
        private void buttonSaveData_Click(object sender, EventArgs e) { 

            string Table = CurrentTable == Kind.BudgetTable ? "Budget" : "Paid";

            var conn = new SQLiteConnection($"Data Source={DBname}");
            conn.Open();

            SQLiteCommand cmd = new SQLiteCommand($"DROP TABLE IF EXISTS {Table}", conn);
            cmd.ExecuteNonQuery();

            cmd.CommandText = DescriptionTable(Table);
            cmd.ExecuteNonQuery();

            switch (CurrentTable) {
                case Kind.BudgetTable:

                    foreach (var val in Budget.List) {
                        cmd.CommandText = 
                            $"INSERT INTO {Table} (FIO, Year_Of_Birth, Prev_Educ, Faculty, Direction, _Group) " +
                            $"VALUES ('{val.FIO}', {val.YearOfBirth}, '{val.PrevEduc}', '{val.Faculty}', " +
                            $"'{val.Direction}', '{val.Group}');";

                        cmd.ExecuteNonQuery();
                    }
                    break;

                case Kind.PaidTable:

                    foreach (var val in Paid.List) {
                        cmd.CommandText =
                            $"INSERT INTO {Table} (FIO, Year_Of_Birth, Prev_Educ, Faculty, Direction, _Group) " +
                            $"VALUES ('{val.FIO}', {val.YearOfBirth}, '{val.PrevEduc}', '{val.Faculty}', " +
                            $"'{val.Direction}', '{val.Group}');";

                        cmd.ExecuteNonQuery();
                    }
                    break;
            }

            conn.Close();

            // Деактивируем кнопку "СОХРАНИТЬ"
            buttonSaveData.Enabled = false;
            buttonSaveData.BackColor = Color.FromArgb(255, 192, 192);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e) {
            
            if(buttonSaveData.Enabled == true) {
                ShowDialogSaveOrDecly();
            }
        }
    }
}
