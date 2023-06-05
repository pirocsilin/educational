
using System.Windows.Forms;
using System.Drawing;

namespace Programming_Tech_Form {
    public partial class AboutBox : Form {

        Form basePtr;

        public AboutBox(Form base_) {

            InitializeComponent();

            basePtr = base_;

            label1.Text = 
                "1.  Программа КОНВЕРТЕР предназначена для преобразования действительного числа\n" +
                 "представленного в системе счисления с основанием p1 в число, представленное в\n" +
                 "системе счисления с основанием p2. Основания систем счисления p1, p2 для исходного\n" +
                 "числа и результата преобразования выбираются пользователем из диапазона от 2..16.\n\n" +
                 "2.  Возможность ввода и редактирования действительного числа представленного в системе\n" +
                 "счисления с основанием p1 с помощью командных кнопок и мыши, а также с помощью клавиатуры.\n\n" +
                 "3.  Обеспечивает контекстную помощь по элементам интерфейса.\n\n" +
                 "4.  Просмотр истории сеанса (журнала) работы пользователя с приложением – исходные данные,\n" +
                 "результат преобразования и основания систем счисления, в которых они представлены;";
        }

        private void AboutBox_LocationChanged(object sender, System.EventArgs e) {

            Point newLocation = this.Location;
            newLocation.X -= basePtr.Width;
            basePtr.Location = newLocation;
        }
    }
}
