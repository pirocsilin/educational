package com.first;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.Matrix;
import android.view.Display;
import android.view.View;
import android.widget.TextView;
import android.graphics.Typeface;
import android.app.Activity;
import android.os.Bundle;
import java.lang.Math;

public class MainActivity extends Activity implements SensorEventListener{
    
    // Объявим необходимые переменные
    SensorManager m; 
    Sensor O;
    float xy_angle, xz_angle, zy_angle;
    TextView xyView, xzView, zyView;
    int MaxX;
    int MaxY;
    float GlobalAngle;
    
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(new myDraw(this));
        
        // Получаем менеджер сенсоров
        m = (SensorManager)getSystemService(SENSOR_SERVICE);
        
        // Наши текстовые поля для вывода показаний
        xyView = (TextView) findViewById(R.id.tvX); 
        xzView = (TextView) findViewById(R.id.tvY); 
        zyView = (TextView) findViewById(R.id.tvZ);
    }
    
    @Override
    protected void onStart(){
        super.onStart();
        // Регистрируем слушатель датчика положения
        m.registerListener (this, m.getDefaultSensor(Sensor.TYPE_ORIENTATION), 
        SensorManager.SENSOR_DELAY_NORMAL);
    }
    
    @Override
    protected void onDestroy(){
        //снимаем слушатель датчика положения
        m.unregisterListener(this);
    }

    @Override
        protected void onPause(){
        super.onPause();
        //снимаем слушатель датчика положения
        m.unregisterListener(this);
    }
    
    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {}
    
    @Override
    public void onSensorChanged(SensorEvent event) { 
        //Изменение показаний датчиков
        xy_angle = event.values[0]; //Плоскость XY угол наклона в градусах
        xz_angle = event.values[1]; //Плоскость XZ
        zy_angle = event.values[2]; //Плоскость ZY
        //Полученные значения выводим на экран
        //xyView.setText(String.valueOf(xy_angle));
        //xzView.setText(String.valueOf(xz_angle));
        //zyView.setText(String.valueOf(zy_angle));
    }
    
    public double toRad(int angle){
        return angle * Math.PI / 180;
    }
    
    public float Offset(int angle, int rad){
        
        float rotate = (float)Math.PI * rad;
        
        if(angle>180)
            angle -= 360;
        else 
            rotate *= -1;
        
        return (float)Math.PI * rad * angle / 180 + rotate;
    }
    
    class myDraw extends View{			// Наследуем функционал класса View
        
        final int r1 = 400,				// Задаем радиусы основных окружностей
                  r2 = 45,
                  r3 = 220,
                  r4 = 120,
                  r5 = 30;
        
        final String nesw[] = {"N","E","S","W"};
        final String nesw2[] = {"NE","SE","SW","NW"};
        
        int angle, temp;
        int InCircle, ExCircle;
        Paint p;
        Path SmallArrow, BigArrow, Face, Text, Lines;
        Matrix matrix;
        
        public myDraw(Context context) {	// Создаем объект Компас
            super(context);
            
            Display display = getWindowManager().getDefaultDisplay(); 
            MaxX = display.getWidth();
            MaxY = display.getHeight();
            
            p = new Paint(Paint.ANTI_ALIAS_FLAG);
            SmallArrow = new Path();
            BigArrow = new Path();
            Face = new Path();
            Text = new Path();
            Lines = new Path();
            matrix = new Matrix();
            
            InCircle = MaxX/2-50;
            ExCircle = InCircle-100;
            
            for(int i=0; i<360; i+=5){
                int d1, d2;
                if(i%45==0){
                    d1 = 25;
                    d2 = 40;
                    BigArrow.moveTo((float)Math.cos(toRad(i))*(InCircle+d1)+MaxX/2,(float)Math.sin(toRad(i))*(InCircle+d1)+MaxY/2);
                    BigArrow.lineTo((float)Math.cos(toRad(i))*(ExCircle-d2)+MaxX/2,(float)Math.sin(toRad(i))*(ExCircle-d2)+MaxY/2);
                }

                else{
                    d1 = i%2==0 ? 20 : 0;
                    Face.moveTo((float)Math.cos(toRad(i))*(InCircle+d1)+MaxX/2,(float)Math.sin(toRad(i))*(InCircle+d1)+MaxY/2);
                    Face.lineTo((float)Math.cos(toRad(i))*(ExCircle-d1)+MaxX/2,(float)Math.sin(toRad(i))*(ExCircle-d1)+MaxY/2);
                }
            }
            Face.addCircle((float)MaxX/2, (float)MaxY/2, (float)InCircle, Path.Direction.CW);
            Face.addCircle((float)MaxX/2, (float)MaxY/2, (float)ExCircle, Path.Direction.CW);
            
            SmallArrow.moveTo(MaxX/2+r5, MaxY/2);
            BigArrow.moveTo(MaxX/2+r3, MaxY/2);
            for(int i=45; i<=315; i+=90){
                BigArrow.lineTo((float)Math.cos(toRad(i))*r2+MaxX/2,(float)Math.sin(toRad(i))*r2+MaxY/2);
                BigArrow.lineTo((float)Math.cos(toRad(i+45))*r3+MaxX/2,(float)Math.sin(toRad(i+45))*r3+MaxY/2);
                SmallArrow.lineTo((float)Math.cos(toRad(i))*r4+MaxX/2,(float)Math.sin(toRad(i))*r4+MaxY/2);
                SmallArrow.lineTo((float)Math.cos(toRad(i+45))*r5+MaxX/2,(float)Math.sin(toRad(i+45))*r5+MaxY/2);
            }
            
            Lines.moveTo((float)MaxX/2-r3, (float)MaxY/2);
            Lines.lineTo((float)MaxX/2+r3, (float)MaxY/2);
            Lines.moveTo((float)MaxX/2, (float)MaxY/2-r3);
            Lines.lineTo((float)MaxX/2, (float)MaxY/2+r3);
            Lines.moveTo((float)Math.cos(toRad(45))*r2+MaxX/2,(float)Math.sin(toRad(45))*r2+MaxY/2);
            Lines.lineTo((float)Math.cos(toRad(225))*r2+MaxX/2,(float)Math.sin(toRad(225))*r2+MaxY/2);
            Lines.moveTo((float)Math.cos(toRad(135))*r2+MaxX/2,(float)Math.sin(toRad(135))*r2+MaxY/2);
            Lines.lineTo((float)Math.cos(toRad(315))*r2+MaxX/2,(float)Math.sin(toRad(315))*r2+MaxY/2);
        }
        
        @Override
        protected void onDraw(Canvas canvas) {			// Переопределяем родительский метод onDraw
            canvas.drawARGB(100, 102, 204, 255);
            
            temp = angle;
            angle = 360-(int)xy_angle;
            
            matrix.reset();								// Перезагружаем матрицу
            matrix.setRotate(-temp, MaxX/2, MaxY/2);	// Возвращаем матрицу в начальное положение
            BigArrow.transform(matrix);					// Возвращаем все объекты в начальное положение
            SmallArrow.transform(matrix);				// Применяем матрицу ко всем Path объектам 
            Face.transform(matrix);
            Lines.transform(matrix);
            matrix.reset();
            
            matrix.setRotate(angle, MaxX/2, MaxY/2);	// Поворачиваем матрицу на новый угол
            SmallArrow.transform(matrix);				// Применяем новую матрицу для трансформации Path объекта
            p.setStyle(Paint.Style.FILL);
            p.setStrokeWidth(1);
            p.setColor(Color.RED);
            canvas.drawPath(SmallArrow, p);				// Отрисовка Path объекта
            
            BigArrow.transform(matrix);
            p.setColor(Color.BLACK);
            p.setStrokeWidth(4);

            canvas.drawPath(BigArrow, p);
            p.setStyle(Paint.Style.STROKE);
            canvas.drawPath(BigArrow, p);
            
            p.setStyle(Paint.Style.STROKE);
            p.setStrokeWidth(2);
            Face.transform(matrix);
            p.setColor(Color.GRAY);
            canvas.drawPath(Face, p);
            
            p.setColor(Color.WHITE);
            p.setStrokeWidth(0.5f);
            Lines.transform(matrix);
            canvas.drawPath(Lines, p);
            
            // Рисуем текст по нужным контурам контурам 
            
            int ang;
            p.setTextAlign(Paint.Align.CENTER);
            
            p.setStrokeWidth(2);
            p.setStyle(Paint.Style.FILL);
            p.setTextScaleX(1.1f);
            p.setTextSize(60);
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)r3, Path.Direction.CW);
            ang = angle;
            for(String val : nesw){
                if(val=="N") 
                    p.setColor(Color.YELLOW);
                else
                    p.setColor(Color.BLACK);
                canvas.drawTextOnPath(val , Text, Offset(ang,r3), -5, p);
                ang = (ang + 90) % 360;
            }
            Text.reset();
            
            p.setTextSize(50);
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)r4, Path.Direction.CW);
            p.setTextScaleX(0.8f);
            ang = (angle + 45) % 360;
            for(String val : nesw2){
                canvas.drawTextOnPath(val , Text, Offset(ang,r4), -20, p);
                ang = (ang + 90) % 360;
            }
            Text.reset();
            
            p.setTextSize(45);
            p.setTextScaleX(0.7f);
            p.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.BOLD));
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)r3, Path.Direction.CW);
            ang = angle;
            for(int i=0; i<360; i+=45){
                ang = (angle + i) % 360;
                if(i==0) 
                    canvas.drawTextOnPath("360/0" , Text, Offset(ang,r3),-70, p);
                else
                    canvas.drawTextOnPath(Integer.toString(i) , Text, Offset(ang,r3),-70, p);
            }
            Text.reset();
            
            p.setTextSize(35);
            p.setTextScaleX(0.7f);
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)InCircle, Path.Direction.CW);
            int delta = (angle + 180) % 360;
            ang = delta;
            for(int i=0; i<360; i+=45){
                ang = (delta + i) % 360;
                if(i==0) 
                    canvas.drawTextOnPath("360/0" , Text, Offset(ang,InCircle),-30, p);
                else
                    canvas.drawTextOnPath(Integer.toString(i), Text, Offset(ang,InCircle),-30, p);
            }
            Text.reset();
            
            p.setTextSize(34);
            p.setTextScaleX(0.8f);
            p.setColor(Color.RED);
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)InCircle, Path.Direction.CW);
            ang = delta;
            for(int i=0; i<360; i+=10){
                ang = (delta + i) % 360;
                if(i % 45 != 0)
                canvas.drawTextOnPath(Integer.toString(i) , Text, Offset(ang,InCircle),-20, p);
            }
            p.setTypeface(Typeface.create(Typeface.DEFAULT, Typeface.NORMAL));
            Text.reset();
            
            p.setColor(Color.GREEN);
            Text.addCircle((float)MaxX/2, (float)MaxY/2, (float)ExCircle, Path.Direction.CW);
            ang = angle;
            for(int i=0; i<360; i+=10){
                ang = (angle + i) % 360;
                if(i % 45 != 0)
                canvas.drawTextOnPath(Integer.toString(i) , Text, Offset(ang,ExCircle),50, p);
            }
            p.setTextScaleX(1f);
            Text.reset();
            
            this.invalidate();
        }
    }
}
    