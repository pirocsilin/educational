package com.example.lab_2_classinteraction;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.os.Bundle;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;
import android.widget.RadioButton;

public class MainActivity extends Activity {

    boolean checked;
    Intent intent;
    Button btn;
    TextView msg;
    int REQUEST_CODE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_a);

        btn = findViewById(R.id.btn);           // Получаем объекты по ID из layout файла
        msg = findViewById(R.id.tv);
    }
    public void onRadioButtonClicked(View v){   // Обработка нажатия радиокнопки
        checked = ((RadioButton)v).isChecked();
        switch(v.getId()){
            case R.id.ClassB:
                if(checked){
                    btn.setText("Вызвать класс В");
                    intent = new Intent(this,ClassB.class);
                    REQUEST_CODE = 1;
                } break;                            // В зависимости от нажатой кнопки, создаем intent
            case R.id.ClassC:                       // с соответствующим классом вызова и устанавливаем
                if(checked){                        // REQUEST_CODE для идентификации вернувшегося результата
                    btn.setText("Вызвать класс C");
                    intent = new Intent(this,ClassC.class);
                    REQUEST_CODE = 2;
                } break;
        }
    }

    public void onClick(View v){                            // Вызываем выбранное Activity. Когда Activity закрывается,
        if(checked){                                        // вызывается метод onActivityResult, показывая, что закрылось
            startActivityForResult(intent, REQUEST_CODE);   // Activity, которое мы вызывали методом startActivityForResult.
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {

        if (resultCode == RESULT_OK){                           // Если вызов прошел успешно, то
            String str = data.getStringExtra("message");  // получаем сообщение из data,
            str = str.isEmpty() ? "NO MESSAGE" : str;           // а по requestCode определяем с какого
            switch (requestCode) {                              // Activity вернулся результат
                case 1:
                    msg.setText("Соощение из класса В: "+str);
                    break;
                case 2:
                    msg.setText("Соощение из класса С: "+str);
                    break;
            }
        }
        else{                                                   // Если data == null, выводим сообщение об ошибке.
            msg.setText("");                                    // Например если была нажата кнопка 'BACK' на устройстве
            Toast.makeText(this, "WRONG RESULT", Toast.LENGTH_SHORT).show();
        }
    }
}