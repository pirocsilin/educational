package com.example.lab_3_accaunt;

import android.app.Activity;
import android.os.Bundle;
import android.widget.EditText;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

public class MainActivity extends Activity {

    EditText Login, Password, Id, Priority, Param;
    int valInteger;
    float valFloat;
    String valStr;
    final int ERROR = -1;
    final String    LOGIN = "login",
                    PASSWORD = "password",
                    ID = "id",
                    PRIORITY = "priority",
                    PARAM = "param";

    SharedPreferences shPref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        Login = findViewById(R.id.login);
        Password = findViewById(R.id.password);
        Id = findViewById(R.id.id);
        Priority = findViewById(R.id.priority);
        Param = findViewById(R.id.parametr);
    }

    public void SavePreferences(){
        shPref = getPreferences(MODE_PRIVATE);                // Объект для работы с данными из файла настроек
        Editor ed = shPref.edit();                            // Объект - редактор, для редактирования данных
        ed.putString(LOGIN, Login.getText().toString());      // Редактируем поле LOGIN
        ed.putString(PASSWORD, Password.getText().toString());// Редактируем поле PASSWORD
        try{                                                  // Пытаемся привести ID к типу Integer и записать
            ed.putInt(ID, Integer.valueOf(Id.getText().toString()));
        }
        catch(Exception e){                                   // Если не удалось привести данне к типу Integer,
            ed.putInt(ID, ERROR);                             // то записываем в поле код ошибки (-1)
        }
        try{
            ed.putInt(PRIORITY, Integer.valueOf(Priority.getText().toString()));
        }
        catch(Exception e){
            ed.putInt(PRIORITY, ERROR);
        }
        try{
            ed.putFloat(PARAM, Float.valueOf(Param.getText().toString()));
        }
        catch(Exception e){
            ed.putFloat(PARAM, ERROR);
        }
        ed.commit();                                           // Сохраняем измененнуе данные
    }

    public void LoadPreferences(){
        shPref = getPreferences(MODE_PRIVATE);                 // Объект для работы с данными

        if((valStr = shPref.getString(LOGIN, "")) != "")    // Получаем значение поля по ключу
            Login.setText(valStr);                             // и выводим на соответствующий TexEdit
        Login.setHint("\t\tstring type");

        if((valStr = shPref.getString(PASSWORD, "")) != "")
            Password.setText(valStr);
        Password.setHint("\t\tstring type");

        if((valInteger=shPref.getInt(ID, ERROR)) != ERROR)      // Если в поле не записан код ошибки,
            Id.setText(Integer.toString(valInteger));           // то выводим его значение,
        else
            Id.setText("");                                     // иначе очищаем поле, например при смене
        Id.setHint("\t\tinteger type");                         // ориентации и выводим текст подсказки.

        if((valInteger=shPref.getInt(PRIORITY, ERROR)) != ERROR)
            Priority.setText(Integer.toString(valInteger));
        else
            Priority.setText("");
        Priority.setHint("\t\tinteger type");

        if((valFloat=shPref.getFloat(PARAM, ERROR)) != ERROR)
            Param.setText(Float.toString(valFloat));
        else
            Param.setText("");
        Param.setHint("\t\tfloat type");
    }

    @Override
    protected void onResume() {
        super.onResume();
        LoadPreferences();
    }

    @Override
    protected void onPause() {
        super.onPause();
        SavePreferences();
    }
}