package com.example.lab_3;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.graphics.*;
import android.os.Bundle;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

public class MainActivity extends AppCompatActivity implements View.OnTouchListener {

    final String[] Mass = {"Т","Р","Ю","Ш","Н","И","К","О","В"};
    boolean StartGame = true;
    Runnable Start;
    int Pos = 0;
    Panel GamePanel;
    Letters[] Arr;
    int maxX;
    int maxY;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);
        maxX = displayMetrics.widthPixels;
        maxY = displayMetrics.heightPixels;

        InitArr();
        GamePanel = new Panel(this);        // Создаем объект GamePanel.
        GamePanel.setOnTouchListener(this);        // Крепим к нему слушателя касания.

        setContentView(GamePanel);                 // Устанавливаем GamePanel как визуальное
                                                   // представление текущей активности.
        Start = new Runnable() {
            @Override
            public void run() {
                Log.v("Tag", "msg = "+"Start_Thread_Main");
                while (StartGame){
                    GamePanel.invalidate();
                }
                Log.v("Tag", "msg = "+"Stop_Thread_Main");
            }
        };
        Thread ThreadStart = new Thread(Start);
        ThreadStart.start();                        // Запускаем поток перерисовки GamePanel.
    }

    public void InitArr(){                          // Инициализация массива объектов - символов.
        Arr = new Letters[Mass.length];
        for(int i=0; i<Mass.length; i++){
            Arr[i] = new Letters(0,0,Mass[i],false);
        }
    }

    @Override                                       // Переопределяем метод onTouch.
    public boolean onTouch(View v, MotionEvent e) {
        if(e.getAction() == MotionEvent.ACTION_DOWN && Arr[Pos].isActive == false){
            int i = Pos;
            Pos = (Pos + 1) % Arr.length;           // Если очередной объект в массиве
            Runnable runnable = new Runnable() {    // свободен в данный момент, то
                @Override                           // создаем новый поток для него.
                public void run() {
                    Log.v("Tag", "msg = "+"Start_Thread_"+i);
                    float Delta;
                    Arr[i].isActive = true;
                    float X = e.getX();
                    float Height = e.getY();

                    for(int Count=1; Count<=4; Count++){    // Поток изменяет координаты объекта
                                                            // по описаному алгоритму.
                        Delta = (maxY - 150 - Height) / 5;

                        for(float Y=Height; Y<=maxY-150; Y++){

                            if(Y <= Height + Delta / 2)
                                Y = Y - 0.4f;
                            if(Y <= Height + Delta)
                                Y = Y + 0.0f;
                            else if(Y <= Height + 2*Delta)
                                Y = Y + 0.4f;
                            else if(Y <= Height + 3*Delta)
                                Y = Y + 0.6f;
                            else if(Y <= Height + 4*Delta)
                                Y = Y + 0.8f;
                            else
                                Y = Y + 1.0f;
                            X += 0.1;

                            Arr[i].posX = X;
                            Arr[i].posY = Y;

                            try {
                                Thread.sleep(2);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }

                        if(Count == 4){
                            try {
                                Thread.sleep(700);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                            break;
                        }

                        Height += ((maxY - Height) / 2)*0.8;
                        Delta = (maxY - 150 - Height) / 5;

                        for(float Y=maxY-150; Y>=Height; Y--){

                            if(Y >= Height + 4*Delta)
                                Y = Y - 1.0f;
                            else if(Y >= Height + 3*Delta)
                                Y = Y - 0.8f;
                            else if(Y >= Height + 2*Delta)
                                Y = Y - 0.6f;
                            else if(Y >= Height + Delta)
                                Y = Y - 0.4f;
                            else if(Y >= Height + Delta/2)
                                Y = Y - 0.0f;
                            else
                                Y = Y + 0.4f;
                            X += 0.1;

                            Arr[i].posX = X;
                            Arr[i].posY = Y;

                            try {
                                Thread.sleep(2);
                            } catch (InterruptedException e) {
                                e.printStackTrace();
                            }
                        }
                    }
                    Arr[i].isActive = false;
                    Log.v("Tag", "msg = "+"Stop_Thread_"+i);
                }
            };
            Thread thread = new Thread(runnable);
            thread.start();                             // Запускаем созданный поток.
        }
        return false;
    }

    class Panel extends View{                           // Panel наследуем от View.
        Paint p;                                        // Это дает доступ к канве
                                                        // через переопределение родительского
        public Panel(Context context) {                 // метода onDraw(Canvas c).
            super(context);

            p = new Paint();
            p.setTextSize(130);
            p.setStrokeWidth(5);
            p.setColor(Color.MAGENTA);
            p.setStyle(Paint.Style.FILL_AND_STROKE);
            p.setTypeface(Typeface.create(Typeface.MONOSPACE, Typeface.NORMAL));
        }

        @Override
        protected void onDraw(Canvas c){                // Визуализация процесса.
            c.drawARGB(40, 102, 204, 255);   // В отдельном потоке происходит
                                                        // постоянная перерисовка холста.
            for(int i=0; i<Mass.length; i++){
                c.drawText(Arr[i].symbol, 70+i*110, 150, p);
                if(Arr[i].isActive){
                    p.setColor(Color.GRAY);
                    c.drawText(Arr[i].symbol, 70+i*110, 150, p);
                    p.setColor(Color.MAGENTA);

                    c.drawText(Arr[i].symbol, Arr[i].posX, Arr[i].posY,p);
                }
            }
        }
    }

    class Letters{                      // Класс Letters - описание объекта.
        public float posX;
        public float posY;
        public String symbol;
        public boolean isActive;

        public Letters(float X, float Y, String str, boolean Active){
            posX = X;
            posY = Y;
            symbol = str;
            isActive = Active;
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        StartGame = false;              // Завершаем поток отрисовки канвы.
    }
}