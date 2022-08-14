using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace Contr
{
    public partial class Form1 : Form
    {
        struct Point
        {
            public int x, y, z;

            public Point(int a = 0, int b = 0, int c = 0) { x = a; y = b; z = c; }

        };
        List<Point>  V = new List<Point>(n);                    // Коорд. точек гамильтонова пути для G
        List<Point> _V = new List<Point>(n);                    // Коорд. точек гамильтонова пути для H
        List<Point> EncodV = new List<Point>(n);                // Список точек и кодир-х значений для Боба

        private int[,] G =  { {0, 0, 1, 0, 0, 1, 1, 1},         // Исходные данные
				            {0, 0, 1, 1, 0, 0, 0, 1},
				            {1, 1, 0, 0, 1, 1, 1, 0},
				            {0, 1, 0, 0, 0, 1, 0, 0},
				            {0, 0, 1, 0, 0, 0, 1, 1},
				            {1, 0, 1, 1, 0, 0, 0, 0},
				            {1, 0, 1, 0, 1, 0, 0, 0},
				            {1, 1, 0, 0, 1, 0, 0, 0} };

        const int n = 8,
                  N = 13 * 47,
                  d = 7;

        List<int> path = new List<int>(n);
        List<int> nodes = new List<int>(n) { 0, 0, 0, 0, 0, 0, 0, 0 };

        int[]  Peaks = { 6, 3, 4, 2, 0, 1, 7, 5 };  // Исходная перестановка вершин
        int[,] EncodPath = new int[n, n];           // Здесь будем хранить шифрованный путь
        int[,] H = new int[n, n];                   // Изоморфный граф
        int[,] _H = new int[n, n];                  // Кодированная матрица
        int[,] F = new int[n, n];                   // Шифрованная матрица

        Graphics gr;                                // Объект для работы с графикой
        int Mode;                                   // Режим взаимодействия Алисы и Боба

        public Form1()
        {
            InitializeComponent();
            gr = CreateGraphics();
        }
        int FastPower(int a, int x, int p)
        {
            int LenBin = (int)(Math.Log(x) / Math.Log(x) + 1), y = 1, s = a;

            while (LenBin-- > 0)
            {
                if ((x & 1) == 1) y = y * s % p;
                s = s * s % p;
                x >>= 1;
            }
            return y;
        }
        private double toRad(int angle){ return angle * Math.PI / 180; } // Значение угла в радианах
        void GetIsomorphicGraph(int[,] A, int[,] B)
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                {                                                       // Используя правила перестановки
                    A[Peaks[i],Peaks[j]] = B[i,j];                      // получаем изоморфный граф 
                    if (V.Contains(new Point(i,j)))                     // и его Гамильтонов цикл
                        EncodPath[Peaks[i],Peaks[j]] = B[i,j];
                }
        }
        void GetEncodedGraph(int[,] G)
        {
            Random rand = new Random(77);
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                {
                    int n = (rand.Next(1, 9)) * 10;
                    _H[i,j] = H[i,j] + n;                               // Кодируем изоморфный граф
                    if (EncodPath[i,j] == 1)
                    {
                        EncodPath[i,j] += n;                            // и его Гамильтонов цикл
                        EncodV.Add(new Point(i, j, EncodPath[i,j]));    // + формируем список для Боба
                        _V.Add(new Point(i, j));                        // Получаем гамильтонов цикл H
                    }
                }
        }
        void GetEncryptedGraph(int[,] A, int[,] B)                      // Шифруем изоморфный граф
        {
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    A[i,j] = FastPower(B[i,j], d, N);
        }
        bool CheckGraph(int v, int[,] G)            // Проверка гамильтонова пути в исходном графе G
        {

            path.Add(v);
            if (path.Count == n)
                if (G[path[0], path[n - 1]] == 1)
                    return true;
                else
                {
                    path.Remove(path.Last());
                    return false;
                }

            nodes[v] = 1;                           // Отмечаем вершину как посещенную
            for (int i = 0; i < n; i++)             // Перебираем вершины для поиска дальнейшего пути
                if (G[v,i]==1 && nodes[i] == 0)     // Если есть ребро и вершину еще не посещали
                    if (CheckGraph(i, G))           // то пытаемся найти продолжение пути от неё
                        return true;

            nodes[v] = 0;
            path.Remove(path.Last());
            return false;
        }

        private void DrawMatrix(int[,] G, Color C, string Name, int dX, int dY, int dL, string Text = "")
        {
            Pen pen = new Pen(C);
            Font font = new Font("Arial", 11);
            pen.Width = 1;

            string str = "";
            for(int i=0; i<n; i++)
            {
                for(int j=0; j<n; j++)
                {
                    str += G[i, j] + "  ";
                }
                str += "\n";
            }
            gr.DrawString(Text, font, new SolidBrush(Color.OrangeRed), 90 + dX, 280 + dY - 27);
            gr.DrawString(str, font, new SolidBrush(C), 90 + dX, 280 + dY);
            gr.DrawString(Name, font, new SolidBrush(C), 50 + dX, 280 + 65 + dY);
            gr.DrawLine(pen, 85 + dX, 280 + dY, 85 + dX, 430 + dY);
            gr.DrawLine(pen, 85 + 144 + dX + dL, 280 + dY, 85 + 144 + dX + dL, 430 + dY);
        }

        private void DrawMatrixF(Color C, int X, int Y, string Text)
        {
            Pen pen = new Pen(C);
            Font font = new Font("Arial", 11);
            pen.Width = 1;

            for (int i = 0; i < n; i++)
            {
                int dY = i * 25, dX;
                for (int j = 0; j < n; j++)
                {
                    dX = j * 40;
                    gr.DrawString(F[i, j].ToString(), font, new SolidBrush(C), X + dX, Y + dY);
                }
            }
            gr.DrawString(Text, font, new SolidBrush(Color.OrangeRed), X+60, Y-25);
            gr.DrawString("F = ", font, new SolidBrush(C), X - 40, Y + 90);
            gr.DrawLine(pen, X - 5, Y, X - 5, Y + 194);
            gr.DrawLine(pen, X + 316, Y, X + 316, Y + 194);
        }

        private void DrawGraph(int[,] G, List<Point> V, int dX, int dY, Color C, string Name, bool Path = true)
        {
            Pen pen = new Pen(C);
            Font font = new Font("Arial", 12);
            gr.DrawString(Name, font, new SolidBrush(C), 120 + dX, 10 + dY);

            for (int i = 0; i < n; i++)                     // Рисуем ребра
            {
                for (int j = 0; j < n; j++)
                {
                    if (V.Contains(new Point(i, j)) && Path)
                    {
                        pen.Width = 3;
                        gr.DrawLine(pen, (float)Math.Cos(toRad(i * 45)) * 100 + 150 + dX,
                                         (float)Math.Sin(toRad(i * 45)) * 100 + 150 + dY,
                                         (float)Math.Cos(toRad(j * 45)) * 100 + 150 + dX,
                                         (float)Math.Sin(toRad(j * 45)) * 100 + 150 + dY);
                    }
                    else if (G[i, j] == 1)
                    {
                        pen.Width = 1;
                        gr.DrawLine(pen, (float)Math.Cos(toRad(i * 45)) * 100 + 150 + dX,
                                         (float)Math.Sin(toRad(i * 45)) * 100 + 150 + dY,
                                         (float)Math.Cos(toRad(j * 45)) * 100 + 150 + dX,
                                         (float)Math.Sin(toRad(j * 45)) * 100 + 150 + dY);
                    }
                }
            }

            pen.Width = 1;
            for (int i = 0; i < 360; i += 45)           // Рисуем вершины
            {
                gr.FillEllipse(new SolidBrush(Color.LightSkyBlue),
                new RectangleF((float)Math.Cos(toRad(i)) * 100 + 140 + dX,
                               (float)Math.Sin(toRad(i)) * 100 + 140 + dY, 20, 20));

                gr.DrawEllipse(pen, (float)Math.Cos(toRad(i)) * 100 + 140 + dX,
                                     (float)Math.Sin(toRad(i)) * 100 + 140 + dY, 20, 20);

                gr.DrawString((i / 45 + 1).ToString(), font, new SolidBrush(Color.Black),
                                                (float)Math.Cos(toRad(i)) * 100 + 142 + dX,
                                                (float)Math.Sin(toRad(i)) * 100 + 140 + dY);

            }
        }
        private void Bbutton1_Click(object sender, EventArgs e)
        {
            Mode = 1;

            AliceText.Text = "Алиса получила вопрос: правда ли что H изоморфен G? Она готова дать ответ";
            BobText.Text = "Боб отправил вопрос и ожидает ответ";
            Header.Text = "Правда ли что H изоморфен G?";
            Abutton.Text = "Отправить H'";
            Bbutton1.Enabled = false;
            Bbutton2.Enabled = false;
            Abutton.Enabled = true;

            gr.Clear(Color.FromArgb(200, 220, 220));
            DrawGraph(H, _V, 120, 140, Color.Green, "Граф H");
            DrawGraph(G, V, 450, 140, Color.Red, "Граф G");
            gr.DrawString("?", new Font("Arial", 25), new SolidBrush(Color.Blue), 423, 275);
        }
        private void Bbutton2_Click(object sender, EventArgs e)
        {
            Mode = 2;

            AliceText.Text = "Алиса получила вопрос: Каков гамильтонов цикл графа H? Она готова ответить";
            BobText.Text = "Боб отправил вопрос и ожидает ответ";
            Header.Text = "Каков гамильтонов цикл для графа H?";
            Abutton.Text = "Отправить ребра H'";
            Bbutton1.Enabled = false;
            Bbutton2.Enabled = false;
            Abutton.Enabled = true;

            gr.Clear(Color.FromArgb(200, 220, 220));
            DrawGraph(H, _V, 220, 140, Color.Green, "Граф H", false);
            gr.DrawString("path ?", new Font("Arial", 20), new SolidBrush(Color.Blue), 510, 270);
        }
        private void button1_Click(object sender, EventArgs e)
        {
            switch (Mode)
            {
                case 0:
                    {
                        AliceText.Text = "Алиса отправила Бобу матрицу F и ждет от него вопрос";
                        BobText.Text = "Боб получил шифрованную матрицу и должен задать один из вопросов";
                        Header.Text = "Боб получил F и готов задать вопрос";
                        Abutton.Text = "Ожидает вопрос";
                        Abutton.Enabled = false;
                        Bbutton1.Enabled = true;
                        Bbutton2.Enabled = true;
                        Header.Visible = true;

                        gr.Clear(Color.FromArgb(200, 220, 220));
                        DrawMatrixF(Color.Black, 285, 180, "Шифрованная матрица F:");

                    } break;
                case 1:
                    {
                        AliceText.Text = "Алиса отправила Бобу кодированную матрицу и правила перестановки вершин";
                        BobText.Text = "Боб получил H' и проверяет ее соответствие F. Используя " +
                                       "перестановки он должен убедиться в идентичности H' и G";
                        Header.Text = "Боб получил H' и правила перестановки";
                        Abutton.Text = "Ожидает вопрос";
                        Abutton.Enabled = false;
                        Bbutton1.Enabled = true;
                        Bbutton2.Enabled = true;

                        gr.Clear(Color.FromArgb(200, 220, 220));
                        DrawMatrix(_H, Color.Black, "H' = ", 100, -130, 69, "Кодированная матрица H':");
                        
                        string str = "{ ";
                        foreach (int v in Peaks) str += (v+1) + ", ";
                        str = str.Trim(new char[] { ' ',',' });
                        str += " }";

                        gr.DrawString(str, new Font("Arial", 14), new SolidBrush(Color.Blue), 460, 200);

                        bool Check = true;
                        str = "True [Матрица H' соответствует F]";
                        int[,] TempMatrix = new int[n, n];                  // проверка идентичности матриц
                        GetEncryptedGraph(TempMatrix, _H);
                        for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n; j++)
                                if ((F[i, j] ^ TempMatrix[i, j]) != 0)
                                {
                                    str = "False [Матрица H' не соответствует F]";
                                    Check = false;
                                    break;
                                }
                            if (!Check) break;
                        }
                        gr.DrawString("Проверка 1: "+str, new Font("Arial", 12), new SolidBrush(Color.Black), 200, 350);

                        Check = true;
                        str = "True [Граф H изоморфен G]";
                        GetIsomorphicGraph(TempMatrix, G);                  // проверка изоморфности графов
                        for (int i = 0; i < n; i++)
                        {
                            for (int j = 0; j < n; j++)
                                if ((_H[i,j] & 1 ^ TempMatrix[i,j]) != 0) 
                                {
                                    str = "False [Граф H не изоморфен G]";
                                    Check = false;
                                    break;
                                }
                            if (!Check) break;
                        }
                        gr.DrawString("Проверка 2: " + str, new Font("Arial", 12), new SolidBrush(Color.Black), 200, 380);

                    } break;
                case 2:
                    {
                        AliceText.Text = "Алиса отправила Бобу кодированный гамильтонов путь графа H в виде списка";
                        BobText.Text = "Боб получил список и проверяет его соответствие F. " +
                                       "Он должен убедиться что путь проходит через все вершины";
                        Header.Text = "Боб получил список кодированных ребер графа H";
                        Abutton.Text = "Ожидает вопрос";
                        Abutton.Enabled = false;
                        Bbutton1.Enabled = true;
                        Bbutton2.Enabled = true;

                        gr.Clear(Color.FromArgb(200, 220, 220));

                        string str = "";
                        foreach (Point v in EncodV) str += "( " + (v.x+1) + ", " + (v.y+1) + ", " + v.z + " );\n";
                        str = str.Trim(new char[] { '\n', ';' });
                        str += '.';

                        gr.DrawString(str, new Font("Arial", 14), new SolidBrush(Color.Blue), 370, 140);

                        str = "True [путь соответствует матрице F]";
                        foreach(Point v in EncodV)
                            if (FastPower(v.z, d, N) != F[v.x, v.y])
                            {
                                str = "False [путь не соответствует матрице F]";
                                break;
                            }
                        gr.DrawString("Проверка 1: " + str, new Font("Arial", 12), new SolidBrush(Color.Black), 200, 350);

                        str = "True [путь проходит через все вершины по одному разу]";
                        foreach (Point A in EncodV)
                        {
                            bool TryPath = false;
                            str = "True [путь является гамильтоновым циклом]";
                            foreach (Point B in EncodV)
                                if ((A.x != A.y) && (B.x != B.y) && (A.x != B.y) && (A.y == B.x))
                                {
                                    TryPath = true;                     // ищем продолжение пути
                                    break;
                                }
                            if (!TryPath) 
                            { 
                                str = "False [путь не является гамильтоновым циклом]";
                                break; 
                            }
                        }
                        gr.DrawString("Проверка 2: " + str, new Font("Arial", 12), new SolidBrush(Color.Black), 200, 380);
                    }
                    break;
            }
        }

        private void Defult_Paint()
        {
            Mode = 0;

            Bbutton1.Enabled = false;
            Bbutton2.Enabled = false;
            Abutton.Enabled = true;
            Header.Visible = false;
            Abutton.Text = "Отправить запрос";
            BobText.Text = "Боб ожидает шифрованную матрицу F от Алисы";
            AliceText.Text = "При нажатии, Алиса инициирует соединение с Бобом отправляя ему матрицу F";

            gr.Clear(Color.FromArgb(200, 220, 220));

            DrawGraph(G, V, 0, 0, Color.Red, "Граф G");
            DrawGraph(H, _V, 250, 0, Color.Green, "Граф H");

            DrawMatrix(G, Color.Black, "G = ", 0, 0, 0);
            DrawMatrix(H, Color.Black, "H = ", 240, 0, 0);
            DrawMatrix(_H, Color.Black, "H' = ", 510, -240, 69, "Кодированная матрица H':");
            DrawMatrixF(Color.Black, 545, 240, "Шифрованная матрица F:");
        }

        private void RestartButton_Click(object sender, EventArgs e)
        {
            Defult_Paint();
        }

        private void label2_Click(object sender, EventArgs e)       // Начало работы
        {
            START_Lable.Visible = false;

            for (int i = 0; i < n; i++)
            {
                Peaks[i] = (Peaks[i] + 2) % 8;            // Вариант 1, но так как счет вершин от 0, то еще +1
            }

            if (!CheckGraph(0, G)) {
                Header.Visible = true;
                Header.Text = "Исходный граф не имеет Гамильтонова пути. Проверьте данные!";}
            else
            {
                for (int i = n - 1; i < 2 * n - 1; i++)             // Координаты точек Гамильтонова пути                     
                    V.Add(new Point(path[i % 8], path[(i + 1) % 8]));

                GetIsomorphicGraph(H, G);                           // Строим изоморфный граф
                GetEncodedGraph(H);                                 // Кодируем изоморфный граф
                GetEncryptedGraph(F, _H);                           // Шифруем кодированный граф

                RestartButton.Visible = true;
                Abutton.Visible = true;
                AliceText.Visible = true;
                Bbutton1.Visible = true;
                Bbutton2.Visible = true;
                BobText.Visible = true;

                Defult_Paint();
            }
        }

        private void Form1_Load(object sender, EventArgs e)         // Запуск приложения
        {
            Abutton.Visible = false;
            AliceText.Visible = false;
            Bbutton1.Visible = false;
            Bbutton2.Visible = false;
            RestartButton.Visible = false;
            BobText.Visible = false;
            Header.Visible = false;
        }
    }
}
