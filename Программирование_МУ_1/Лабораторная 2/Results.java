package com.example.lab_2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class Results extends AppCompatActivity {

    String loadRes;
    LinearLayout newText;
    LinearLayout textPanel;
    LinearLayout.LayoutParams params;
    SharedPreferences Res;
    TextView text;
    final String[] HeaderText = new String[]{"Игрок:", "Жуки/Очки:", "Дата:"};
    final String RESULTS_KEY = "results";
    final String RESULTS_FILE = "resultsFile";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_results);

        textPanel = findViewById(R.id.TextPanel);

        Res = getSharedPreferences(RESULTS_FILE,MODE_PRIVATE);  // Получаем данные из файла с именем
        loadRes = Res.getString(RESULTS_KEY,"");             // "resultsFile" и сохраняем
                                                                // содержимое с ключом "results"
        Log.v("Tag1","msg: "+loadRes);                 // в строку loadRes.

        if(loadRes.isEmpty()){                                  // Если файл пуст - "НЕТ ДАННЫХ".
            CreateNewStroke();
            AddNewStroke("НЕТ ДАННЫХ", 20, 1, Color.GRAY);
            textPanel.addView(newText);
        }
        else{                                                       // Иначе создаем заголовок.
            CreateNewStroke();
            for(int i=0; i<3; i++){
                AddNewStroke(HeaderText[i], 20, 1, Color.RED);
            }
            textPanel.addView(newText);                             // Вставляем заголовок.

            CreateNewStroke();                                      // Вставляем пустую строку.
            AddNewStroke("", 5, 1, 0);
            textPanel.addView(newText);

            for (String oneStroke : loadRes.split("//")){     // Парсим рстроку loadRes по
                int i = 0;                                          // разделителям " " и "//",
                CreateNewStroke();                                  // формируем строки результатов
                for (String data : oneStroke.split(" ")){     // и вставляем.
                    AddNewStroke(data, 15, i, Color.GRAY);
                    i++;
                }
                textPanel.addView(newText);
            }
        }
    }
    public void CreateNewStroke(){                          // Создаем новый контейнер для
        newText = new LinearLayout(this);            // очередной результирующей строки.
        newText.setOrientation(LinearLayout.HORIZONTAL);
        params = new LinearLayout.LayoutParams
                (LinearLayout.LayoutParams.WRAP_CONTENT, LinearLayout.LayoutParams.WRAP_CONTENT);
        params.weight = 1;
    }
    public void AddNewStroke(String data, int TextSize, int i, int Color){
        text = new TextView(this);
        text.setText(data);                                 // Создаем текстовое поле для каждой
        text.setTextSize(TextSize);                         // распарсеной строки и добавляем
        text.setTextColor(Color);                           // в контейнер результата.
        switch (i){
            case 1:
            case 2: text.setTextAlignment(View.TEXT_ALIGNMENT_CENTER);
                break;
        }
        text.setWidth(0);
        newText.addView(text, params);
    }

    public void onNumberClick(View view){                   // Обработчик нажатий на кнопки.
        Button button = (Button)view;
        switch (button.getText().toString()){
            case "ОЧИСТИТЬ":                                // Удаляем все результаты.
                if(!loadRes.isEmpty()){
                    SharedPreferences.Editor editor = Res.edit();
                    editor.remove(RESULTS_KEY);
                    editor.apply();
                    loadRes = "";
                    textPanel.removeAllViews();
                    CreateNewStroke();
                    AddNewStroke("НЕТ ДАННЫХ", 20, 1, Color.GRAY);
                    textPanel.addView(newText);
                    Log.v("Clear","msg: "+loadRes);
                }
                break;
                                                            // Возврат в стартовую активность.
            case "ВОЗВРАТ":
                Intent intent = new Intent(this, MainActivity.class);
                startActivity(intent);
                break;
        }
    }
}