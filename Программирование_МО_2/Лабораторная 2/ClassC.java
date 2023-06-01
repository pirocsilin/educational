package com.example.lab_2_classinteraction;

import android.app.Activity;
import android.content.Intent;
import android.view.View;
import android.os.Bundle;
import android.widget.EditText;

public class ClassC extends Activity {

    EditText txt;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_c);

        txt = findViewById(R.id.msgText);
    }
    public void onClick(View v) {
        Intent intent = new Intent();
        intent.putExtra("message", txt.getText().toString());
        setResult(RESULT_OK, intent);
        finish();
    }
}
