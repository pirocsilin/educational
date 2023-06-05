
using System.Drawing;
using System.Windows.Forms;

namespace Programming_Tech_Form {
    public partial class HistoryBox : Form {

        Form basePtr;
        
        public HistoryBox(Form base_) {

            InitializeComponent();
            basePtr = base_;
        }

        // Перемещение главного окна приложения при изменении локации текущей формы
        private void HistoryBox_LocationChanged(object sender, System.EventArgs e) {
            Point newLocation = this.Location;
            newLocation.X -= basePtr.Width;
            basePtr.Location = newLocation;
        }
    }
}
