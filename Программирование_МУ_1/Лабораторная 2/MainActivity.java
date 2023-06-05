package com.example.lab_2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.EditText;

public class MainActivity extends AppCompatActivity{
    String NAME;
    String SETTINGS;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void ClickButtonStart(View v){
        int NumBugs=0;
        boolean ErrorFormat = false;

        EditText name = findViewById(R.id.textName);
            char[] DraftName = name.getText().toString().toCharArray();
        EditText settings = findViewById(R.id.textNum);
            SETTINGS = settings.getText().toString();
        try {
            NumBugs = Integer.valueOf(SETTINGS);            // Приводим введенные пользователем
        }                                                   // данные в требуемый формат,
        catch (NumberFormatException e) {                   // если не удается, ругаемся.
            ErrorFormat = true;
        }
        NAME = "";
        for (char ch : DraftName){
            switch ((int)ch){
                case 10:
                case 32:
                case 47: continue;
            }
            NAME += ch;
            Log.v("TagS", "name = "+NAME);
            if(NAME.length()>=10){
                break;
            }
        }
        Log.v("Tag3", "name = "+NAME);
        if(NAME.isEmpty()){
            name.setError("Задайте имя игрока\nмаксимум 10 символов");
            Log.v("Tag", "Имя не задано");
        }
        else if(NumBugs < 10 || NumBugs > 50 || ErrorFormat){
            settings.setError("Количество от 10 до 50");
            Log.v("Tag", "Количество не верно");
        }
        else{
            Intent intent = new Intent(this, StartGame.class);
            intent.putExtra("NAME_KEY", NAME);
            intent.putExtra("NUM_KEY", NumBugs);                   // Запуск игровой Activity.
            Log.v("Tag", "Put: "+NAME+", "+NumBugs);
            startActivity(intent);
            Log.v("Tag", "Запуск игры в новой активити");
        }
    }

    public void ClickButtonResult(View v) {                             // Переход к активности
        Intent intent = new Intent(this, Results.class);    // просмотра результатов.
        startActivity(intent);
    }
}