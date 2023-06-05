package com.example.lab_2_classinteraction;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.os.Bundle;
import android.widget.EditText;

public class ClassB extends Activity {

    EditText txt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_b);

        txt = findViewById(R.id.msgText);
    }
    public void onClick(View v) {
        Intent intent = new Intent();                               // Создаем намерение и помещаем в него
        intent.putExtra("message", txt.getText().toString()); // данные из txt под именеи 'message'
        setResult(RESULT_OK, intent);                               // Метод setResult адресует данное намерение
        finish();                                                   // в родительское Activity, в котором был
    }                                                               // вызван метод startActivityForResult
}
