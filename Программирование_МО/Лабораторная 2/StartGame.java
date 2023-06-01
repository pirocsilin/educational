package com.example.lab_2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.*;
import android.media.MediaPlayer;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;
import java.security.SecureRandom;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.Random;

public class StartGame extends AppCompatActivity implements View.OnTouchListener{

    final String RESULTS_KEY = "results";
    final String RESULTS_FILE = "resultsFile";
    final int Timer = 30000;
    final byte Rolling = 8;
    MediaPlayer mPlayer;
    MediaPlayer fastSound;
    CountDownTimer T;
    boolean StartGame = false;
    boolean GameOver = false;
    int CountTouch = 0;
    String EndTimeGame;
    String GamerName;
    Runnable runnable;
    Runnable runnable2;
    Panel GamePanel;
    int numBuds;
    int CountBugs;
    int maxX;
    int maxY;
    Bug[] Arr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        maxX = displayMetrics.widthPixels;
        maxY = displayMetrics.heightPixels;                       // Получаем границы дисплея.

        mPlayer = MediaPlayer.create(this, R.raw.intro01); // Инициализируем объект медиаплеер.

        GamePanel = new Panel(this);              // Создаем объект нашей игровой панели
        GamePanel.setOnTouchListener(this);              // и крепим к нему слушателя касания.

        Bundle args = getIntent().getExtras();           // Получаем имя игрока и количество
        GamerName = args.getString("NAME_KEY");      // насекомых из вызвавшей активности.
        numBuds = args.getInt("NUM_KEY");
        Log.v("num", "numBuds = "+numBuds+", name = "+GamerName);
        CountBugs = numBuds;
        Arr = new Bug[numBuds];

        MassBugsInit();
                                                                // Устанавливаем игровую панель
        setContentView(GamePanel);                              // как текущую активность.

        T = new CountDownTimer(Timer, 1000) {     // Создаем таймер.
            Thread thread2;
            public void onTick(long millisUntilFinished) {
                if(!StartGame){
                    StartGame = true;
                    mPlayer.start();                            // Включаем фоновую музыку.
                    for(int i = 0; i<Arr.length; i++){          // Создаем и запускаем потоки для
                        CreateAndStartChangeXY(i);              // пересчета координат каждого жука.
                    }
                    thread2 = new Thread(runnable2);            // Создаем и запускаем поток
                    thread2.start();                            // перерисовка GamePanel.
                    Log.v("Init: ", "Start_Timer");
                }
                EndTimeGame = String.valueOf(millisUntilFinished / 1000);
                if(CountBugs == 0){
                    cancel();                                // Остановка таймера если все насекомые
                    onFinish();                              // мертвы и переход к onFinish().
                }
            }
            public void onFinish() {                         // По завершении таймера
                mPlayer.stop();                              // останавливаем все потоки
                StartGame = false;                           // и переходим к стартовой активности.
                GameOver = true;
                Log.v("End: ", "End_Timer");
                Toast toast = Toast.makeText(StartGame.this, "КОНЕЦ ИГРЫ!", Toast.LENGTH_LONG);
                toast.show();
                try {
                    Thread.sleep(4000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Intent intent = new Intent(StartGame.this, MainActivity.class);
                startActivity(intent);
            }
        }.start();

        runnable2 = new Runnable() {            // Создаем объект интерфейса Runnable
            @Override                           // и переопределяем метод run() в котором
            public void run() {                 // будем перерисовывать GamePanel пока
                while (StartGame){              // счетчик не изменит статус игры на
                    GamePanel.invalidate();     // StartGame = false.
                }
            }
        };
        Log.v("Init: ", "Finish_MAIN");
    }

    public void CreateAndStartChangeXY(int i){
        runnable = new Runnable() {             // Аналогично, переопределяем run() в
            @Override                           // котором будем постоянно пересчитывать
            public void run() {                 // координаты каждого объекта - жука,
                int posX = maxX - 200;          // при условии что он жив.
                int posY = maxY - 200;
                Log.v("ChangeXY: ", "Start_Thread_"+i);
                Random r = new Random();
                while (StartGame){                      // Здесь изменяем угол повора
                                                        // объекта на 0,5 градуса. Каждый объект
                        if(Arr[i].Alive){               // будет поворачиваться создавая эффект
                            if(Arr[i].Up == true){      // сложного движения (ползанья) в диапазоне
                                Arr[i].Delta += 0.5;    // +/- 8 градусов от своей траектории движения.
                                if(Arr[i].Delta > Rolling){
                                    Arr[i].Up = false;
                                    Arr[i].Down = true;
                                }
                            }
                            if(Arr[i].Down == true){
                                Arr[i].Delta -= 0.5;
                                if(Arr[i].Delta < -Rolling){
                                    Arr[i].Down = false;
                                    Arr[i].Up = true;
                                }
                            }
                            Log.v("Delta: ", "["+i+"] = "+Arr[i].Delta);

                            switch((int)Arr[i].Degrees){    // Теперь изменяем координаты объекта
                                                            // в зависимости от его направленности.
                                case 270: Arr[i].posY -= 1;
                                    if(Arr[i].posY < 1){
                                        Arr[i].posY = maxY;
                                        Arr[i].posX = r.nextInt(posX)+100;
                                    }
                                    break;

                                case 180: Arr[i].posX -= 1;
                                    if(Arr[i].posX < -70){
                                        Arr[i].posX = maxX + 70;
                                        Arr[i].posY = r.nextInt(posY)+100;
                                    }
                                    break;

                                case 90:  Arr[i].posY += 1;
                                    if(Arr[i].posY > maxY) {
                                        Arr[i].posY = -70;
                                        Arr[i].posX = r.nextInt(posX)+100;
                                    }
                                    break;

                                case 0: Arr[i].posX += 1;
                                    if(Arr[i].posX > maxX){
                                        Arr[i].posX = -70;
                                        Arr[i].posY = r.nextInt(posY)+100;
                                    }
                                    break;
                            }
                        }
                    try {
                        Thread.sleep(5);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }
                Log.v("ChangeXY: ", "Stop_Thread_"+i);
            }
        };
        Thread thread = new Thread(runnable);
        thread.start();
    }

    public void BloodDrops(int i){                // Метод вызывается если попали
        CountBugs--;                              // по жуку. Включаем быструю мелодию -
        CountTouch ++;                            // хруст жука, далее создаем и запускаем
        FastSound(R.raw.yahoo);                   // поток отвечающий за отрисовку кровавого
        Arr[i].Alive = false;                     // пятна с небольшой задержкой.
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                Arr[i].Blood = true;
                Log.v("Bld: ", " ["+i+"]: "+ Arr[i].Blood);
                try {
                    Thread.sleep(1000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Arr[i].Blood = false;
                Log.v("Bld: ", " ["+i+"]: "+ Arr[i].Blood);
            }
        };
        Thread thread3 = new Thread(runnable);
        thread3.start();
    }
    public void MassBugsInit(){                   // Инициализация массива объектов - жуков.
        SecureRandom random = new SecureRandom();
        int res=0;
        int Degrees=0;
        for(int i=0; i<numBuds; i++){
            switch (random.nextInt(2)){     // В случайном порядке выбираем
                case 0: res = R.drawable.cow;     // одного из двух насекомых.
                        break;
                case 1: res = R.drawable.fly;
                        break;
            }
            int posX = maxX - 200;
            int posY = maxY - 200;
            posX = random.nextInt(posX) + 100;                    // Задаем случайную позицию
            posY = random.nextInt(posY) + 100;                    // объекта на экране и
            int Delta = random.nextInt(Rolling*2)-Rolling;  // случайный угол поворота.

            Degrees = (Degrees + 90) % 360;       // Задаем угол расположения кратный 90 град.
            Arr[i] = new Bug(res, posX,posY,Degrees,Delta,true,false,0,0, true, false);
            Log.v("Pos: ", "Position: "+posX+", "+posY+", "+Degrees);
        }
    }

    @Override                                                       // Переопределяем метод касания.
    public boolean onTouch(View v, MotionEvent e) {
        if(e.getAction() == MotionEvent.ACTION_DOWN && GameOver == false){
            float X0 = e.getX();
            float Y0 = e.getY();                                    // Если произошло касание и игра запущена
            boolean Miss = true;                                    // то проверяем координаты касания и позиции
            for(int i=0; i<numBuds; i++){                           // всех объектов. При попадании
                Log.v("Alive", "["+i+"]: "+Arr[i].Alive);  // рисуем кролвь и вкл быструю музыку.
                if(Arr[i].Alive == true){
                    switch ((int)Arr[i].Degrees){
                        case 0: if(X0>Arr[i].posX && X0<Arr[i].posX+Arr[i].Width
                                && Y0>Arr[i].posY && Y0<Arr[i].posY+Arr[i].Height){
                            BloodDrops(i);
                            Miss = false;
                        }
                            break;

                        case 90: if(X0>Arr[i].posX-Arr[i].Height && X0<Arr[i].posX
                                && Y0>Arr[i].posY && Y0<Arr[i].posY+Arr[i].Width){
                            BloodDrops(i);
                            Miss = false;
                        }
                            break;

                        case 180: if(X0>Arr[i].posX-Arr[i].Width && X0<Arr[i].posX
                                && Y0>Arr[i].posY-Arr[i].Height && Y0<Arr[i].posY){
                            BloodDrops(i);
                            Miss = false;
                        }
                            break;

                        case 270: if(X0>Arr[i].posX && X0<Arr[i].posX+Arr[i].Height
                                && Y0>Arr[i].posY-Arr[i].Width && Y0<Arr[i].posY){
                            BloodDrops(i);
                            Miss = false;
                        }
                            break;
                    }
                }
            }
            if(Miss){                           // Если не попали по жуку, то
                CountTouch --;                  // включаем музыку промаха.
                FastSound(R.raw.luser);
            }
        }
        return false;
    }
    public void FastSound(int SoundCode){                       // Создаем отдельный поток для каждого
        fastSound = MediaPlayer.create(this, SoundCode); // проигрывания быстрой мелодии:
        Runnable runnable = new Runnable() {                    // попадание по жуку или промах.
            @Override
            public void run() {
                fastSound.start();
            }
        };
        Thread thread = new Thread(runnable);
        thread.start();
    }
    class Panel extends View{                                   // Panel наследуем от View
        Paint p;                                                // и получаем доступ к канве
        Bitmap b;                                               // через метод onDraw(Canvas c).
        Matrix m;
        RectF rectF;
        Bitmap Blood;
        boolean Start = false;

        public Panel(Context context) {
            super(context);
            p = new Paint();
            m = new Matrix();
            p.setTextSize(50);
            p.setStrokeWidth(3);
            rectF = new RectF(50, 40, 350, 230);
            Blood = BitmapFactory.decodeResource(getResources(), R.drawable.blood);
        }
        @Override
        public void onDraw(Canvas c){                       // Переопределяем метод для своих нужд:
            c.drawARGB(40, 102, 204, 255);       // отрисока объектов по координатам, а
                                                            // так же результатов игры в отдельном
            for(int i=0; i<numBuds; i++){                   // графическом окне.
                if(Arr[i].Alive == true){
                    b = BitmapFactory.decodeResource(getResources(), Arr[i].type);

                    m.reset();
                    m.preRotate(Arr[i].Degrees+Arr[i].Delta);
                    m.postTranslate(Arr[i].posX, Arr[i].posY);

                    c.drawBitmap(b, m, null);

                    if(!Start){
                        Arr[i].Width = b.getWidth();
                        Arr[i].Height = b.getHeight();
                    }
                }
                else if(Arr[i].Blood == true){
                    m.reset();
                    m.preRotate(Arr[i].Degrees);
                    m.postTranslate(Arr[i].posX, Arr[i].posY);
                    c.drawBitmap(Blood, m,null);
                }
            }
            p.setTextSize(50);
            p.setColor(Color.WHITE);
            p.setStyle(Paint.Style.FILL);
            c.drawRoundRect(rectF,20, 20, p);

            p.setColor(Color.BLACK);
            p.setStyle(Paint.Style.STROKE);
            c.drawRoundRect(rectF,20, 20, p);

            p.setColor(Color.RED);
            p.setStyle(Paint.Style.FILL_AND_STROKE);

            c.drawText("время ",80,95,p);
            c.drawText("жуки  ",80,150,p);
            c.drawText("очки  ",80,205,p);

            p.setColor(Color.BLUE);
            c.drawText(String.valueOf(EndTimeGame),260,95,p);

            p.setColor(Color.MAGENTA);
            c.drawText(String.valueOf(CountBugs),260,150,p);

            if(CountTouch > 0)
                p.setColor(Color.GRAY);
            else
                p.setColor(Color.BLACK);
            c.drawText(String.valueOf(CountTouch),250,205,p);
            Start = true;
        }
    }
    class Bug{                          // Класс хранит состояние объекта:
        public int type;                // координаты, тип изображения, его
        public int posX;                // ширину и высоту, угол поворота,
        public int posY;                // состояние: жив, мертв.
        public float Degrees;
        public float Delta;
        public boolean Up;
        public boolean Down;
        public int Width;
        public int Height;
        public boolean Alive;
        public boolean Blood;
        public Bug(int type, int posX, int posY, float Degrees, float Delta, boolean Up,
                        boolean Down, int Width, int Height, boolean Alive, boolean Blood){
            this.type = type;
            this.posX = posX;
            this.posY = posY;
            this.Degrees = Degrees;
            this.Delta = Delta;
            this.Up = Up;
            this.Down = Down;
            this.Width = Width;
            this.Height = Height;
            this.Alive = Alive;
            this.Blood = Blood;
        }
    }
    @Override
    protected  void onPause() {
        super.onPause();                // Если игра закончена, сохраням результат.
        if(GameOver){
            SimpleDateFormat sdf = new SimpleDateFormat("dd.MM.yy(HH:mm)");
            String DataTime = sdf.format(new Date());

            SharedPreferences Res = getSharedPreferences(RESULTS_FILE,MODE_PRIVATE);
            String loadRes = Res.getString(RESULTS_KEY,"");

            SharedPreferences.Editor editor = Res.edit();
            String Results = loadRes+GamerName+" "+numBuds+"/"+CountTouch+" "+DataTime+"//";
            editor.putString(RESULTS_KEY, Results);
            editor.apply();

            mPlayer.stop();
            Log.v("Pref", " "+Results);
        }
        else                           // Останавливаем звуковое сопровождение.
            mPlayer.pause();

        Log.v("Pause", "Game Over: "+GameOver);
    }
    @Override
    protected void onResume() {        // Если игра еще не закончена
        super.onResume();              // восстанавливаем воспроизведение звука.
        if(!GameOver){
            mPlayer.start();
        }
    }
    @Override
    protected void onDestroy() {
        super.onDestroy();
        StartGame = false;          // Останавливаем потоки: отрисовка
        T.cancel();                 // вычисление позиций жуков и таймер.
    }
}