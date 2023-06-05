package com.example.lab_1_studentslist;

import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Color;
import android.util.Log;
import android.view.View;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.Toast;
import android.view.View.OnClickListener;
import android.content.Context;
import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import java.lang.Math;
import java.util.ArrayList;

public class MainActivity extends AppCompatActivity{

    TableLayout TL;
    TableRow r;
    DBHelper dbHelper;
    SQLiteDatabase db;
    TextView empty;
    TableRow.LayoutParams params;
    final int TextSize = 16, HeaderSize = 18, TextColor = Color.GRAY,  HeaderColor = 0xEB952020;
    final String head[] = {"№", "Имя", "Вес", "Рост", "Возраст"};
    final String chels[] = {"Кирилл", "Антон", "Дмитрий", "Валентин", "Семен", "Марина", "Екатерина",
                          "Александр", "Евгений", "Николай", "Петр", "Вадим", "Людмила", "Анастасия",
                          "Елена", "Виктор", "Елисей", "Андрей", "Рауль", "Джеймс", "Виктория",
                          "Евгений", "Анатолий", "Николай", "Екатерина"};

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);            // Установка отображения на файл activity_main.xml

        TL = findViewById(R.id.TL);                        // По ID находим нужные View из activity_main.xml
        empty = findViewById(R.id.EmptyTxt);
        dbHelper = new DBHelper(this);                  // Создаем объект dbHelper для управления БД
        db = dbHelper.getWritableDatabase();               // Подключаемся к базе данных и получаем объект SQLiteDatabase
        dbHelper.onUpgrade(db, 1, 1);   // Обновляем БД (очищаем таблицу 'mytable')
    }

    public void onClick(View v){                          // Анализируем нажатие кнопки
        ContentValues cv = new ContentValues();
        Button btn = (Button) v;

        switch (btn.getText().toString()){
            case "Заполнить":
                dbHelper.onUpgrade(db, 1, 1);                // Обновляем БД
                ArrayList<Integer> l = new ArrayList<Integer>();                // Создаем массив индексов
                while(l.size()<chels.length){                                   // Формируем массив индексов
                    int n = (int)(Math.random()*chels.length);                  // для случайного выбора
                    if(!l.contains(n)) l.add(n);                                // имени студента
                }
                for(int i=0; i<l.size(); i++){
                    cv.put("name", chels[l.get(i)]);                            // Случайный выбор имени
                    cv.put("weight", (int)(Math.random()*(90-70)+70));          // вес от 70 до 90
                    cv.put("height", (int)(Math.random()*(190-160)+160));       // рост от 160 до 190
                    cv.put("age", (int)(Math.random()*(50-20)+20));             // возраст от 20 до 50
                    long rowID = db.insert("mytable",null,cv); // Добавляем запись в БД
                }

                TL.removeAllViews();                    // Очищаем TableLayout
                empty.setText("Список заполнен");       // Подготавливаем и выводим сообщание
                TL.addView(empty);                      // о том что список заполнен

                break;
            case "Показать":
                Cursor c = db.rawQuery("Select * from mytable order by name",null);
                if(c.moveToFirst()){
                    int NameIndex = c.getColumnIndex("name");       // Используя объект Cursor, определяем
                    int WeightIndex = c.getColumnIndex("weight");   // номера столюцов по имени в выборке
                    int HeightIndex = c.getColumnIndex("height");
                    int AgeIndex = c.getColumnIndex("age");

                    TL.removeAllViews();                     // Очищаем TableLayout

                    params = new TableRow.LayoutParams       // Создаем и заполняем объект настроек для TableRow
                            (LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
                    params.weight = 1;

                    r = new TableRow(this);           // Создаем новый объект TableRow для
                    for(String str : head){                  // для заполнения заголовка таблицы
                        AddNewStroke(str, View.TEXT_ALIGNMENT_CENTER,HeaderSize,HeaderColor);
                    }
                    TL.addView(r);                           // Добавляем заголовок к таблице

                    r = new TableRow(this);
                    for(String str : head) {
                        AddNewStroke("", View.TEXT_ALIGNMENT_CENTER, 5, 0);
                    }
                    TL.addView(r);                           // Добавляем пустую строку-отступ к таблице

                    int count = 0;
                    do{                                      // Получаем значения по номерам столбцов, добавляем в TableRow
                        r = new TableRow(this);
                        AddNewStroke(Integer.toString(++count),View.TEXT_ALIGNMENT_CENTER,TextSize,TextColor);
                        AddNewStroke(c.getString(NameIndex),View.TEXT_ALIGNMENT_TEXT_START,TextSize,TextColor);
                        AddNewStroke(c.getString(WeightIndex),View.TEXT_ALIGNMENT_CENTER,TextSize,TextColor);
                        AddNewStroke(c.getString(HeightIndex),View.TEXT_ALIGNMENT_CENTER,TextSize,TextColor);
                        AddNewStroke(c.getString(AgeIndex),View.TEXT_ALIGNMENT_CENTER,TextSize,TextColor);
                        TL.addView(r);                       // Добавляем очередную строку TableRow в Tablelayout
                    }while(c.moveToNext());                  // Продолжаем пока не прочитаем все строки таблицы
                }
                else
                    Toast.makeText(this,"В таблице нет данных",Toast.LENGTH_SHORT).show();
                break;
            case "Очистить":
                dbHelper.onUpgrade(db, 1, 1);   // No comments
                TL.removeAllViews();
                empty.setText("Список пуст");
                TL.addView(empty);
        }
    }

    public void AddNewStroke(String value, int align, int size, int color){
        TextView txt = new TextView(this);
        txt.setText(value);                                 // Формируем новое View - TextView
        txt.setTextAlignment(align);                        // Устанавливаем атрибуты отображения
        txt.setTextSize(size);                              // и добавляем в TableRow
        txt.setTextColor(color);
        r.addView(txt, params);
    }

    class DBHelper extends SQLiteOpenHelper{
        public DBHelper(Context c){
            super(c,"myDB",null,1);
        }
        @Override
        public void onCreate(SQLiteDatabase db){                // Создаем таблицу mytable
            db.execSQL("create table mytable ("
                    + "id integer primary key autoincrement,"
                    + "name text,"
                    + "weight integer,"
                    + "height integer,"
                    + "age integer" + ");");
        }
        @Override
        public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion){
            db.execSQL("DROP TABLE IF EXISTS mytable");
            onCreate(db);                                       // Перезаписываем таблицу mytable
        }
    }
}