package com.example.calculator;

import static java.lang.Math.pow;

import androidx.appcompat.app.AppCompatActivity;
import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity{

    TextView resultField;               // Поле вывода результата.
    TextView numberField;               // Поле ввода числа.

    String X = null, Y = null;          // Операнды.
    String previousOperation = null;    // Предыдущая операция.
    String lastPressButton;             // Последняя нажатия кнопка.
    boolean OperStatus = false;         // Статус операции.
    boolean DivisionByZero = false;     // Флаг деление на ноль

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);            
        setContentView(R.layout.activity_main);        // Установка разметки для текущего
                                                       // объекта activity.
        resultField = findViewById(R.id.resultField);  // Инициализация полей.
        numberField = findViewById(R.id.numberField);
    }
    public void onNumberClick(View view){               // Получаем нажатую кнопку.
        Button button = (Button)view;                   // Приводим к типу Button.
        String Symbol = button.getText().toString();    // Извлекаем текст кнопки.

        int Size = resultField.getText().toString().length();                   
        if(Size !=0 && resultField.getText().toString().charAt(Size-1) == 	// Получаем длину поля
			'=' && DivisionByZero == false){ 								// resultField и
            Symbol = "C";                                                   // анализируем.
            Log.v("MyTag", "Поле result имеет последний символ: '='");
        }
        if(DivisionByZero == true){
            numberField.setText("");
            resultField.setText("");
            lastPressButton = "Number";
            DivisionByZero = false;
        }
        else if(lastPressButton == "Operation") {    // Если последнее нажатие это
            if(OperStatus == true){                  // операция, то обнуляем поле
                //resultField.setText(" ");          // и формируем второй операнд.
                char ch = resultField.getText().toString().charAt(Size-1);
                resultField.setText(" " + numberField.getText() + " " + ch);
                OperStatus = false;
                Log.v("MyTag", "Выполнена операция");
            }
            numberField.setText("");
            lastPressButton = "Number";
            Log.v("MyTag", "Операция еще не выполнена");
        }
        switch (Symbol){                        // Анализ полученного символа.
            case ".":{
                int FindPoint = numberField.getText().toString().indexOf(".");
                if(FindPoint != -1)
                    return;
                if(numberField.getText().toString().isEmpty() ||
                        numberField.getText().toString().equals("0")) {
                    numberField.setText("0.");
                    return;
                }
            }
            case "0":{
                if(numberField.getText().toString().isEmpty() ||
                        numberField.getText().toString().equals("0")){
                    numberField.setText("0");
                    return;
                }
                break;
            }
            case "C": {
                numberField.setText(""); numberField.setHint("0");
                resultField.setText(""); resultField.setHint("=");
                X = null; Y = null; previousOperation = null;
                return;
            }
            default:{
                if(numberField.getText().toString().equals("0"))
                    numberField.setText("");
            }
        }
        numberField.append(button.getText());
    }
    @SuppressLint("SetTextI18n")
    public void onOperationClick(View view){            // Аналогично, получаем кнопку,
        Button button = (Button)view;                   // далее анализ состояний
        String Operation = button.getText().toString(); // и текста.

        if(numberField.getText().toString().isEmpty()) {
            resultField.setText("0 " + Operation);
            numberField.setText("0");
            X = "0";
            Log.v("MyTag", "Числовое поле пустое");
        }
        else if(OperStatus == true){
            resultField.setText(numberField.getText() + " " + Operation);
            OperStatus = false;
        }
        else if(lastPressButton == "Operation"){
            int Size = resultField.getText().toString().length();
            String R = resultField.getText().toString().substring(0, Size-2);
            resultField.setText(R + " " +button.getText());
            Log.v("MyTag", "Повторное нажатие кнопки операции");
        }

        else{
            resultField.append(" " + numberField.getText() + " " + Operation);
            if(X == null)
                X = numberField.getText().toString();
            else
                Y = numberField.getText().toString();
            Log.v("MyTag", "Добавили в поле result: Операнд + операция");
        }

        if(X != null && Y != null){
            performOperation(X, Y, previousOperation);
        }
        previousOperation = Operation;
        lastPressButton = "Operation";
    }
    public void performOperation(String a, String b, String op) {   // Выполнение арифм
        double x = Double.valueOf(a);                               // операции.
        double y = Double.valueOf(b);           // Операнды приводим к типу double

        switch (op) {                           // Выполняем действие в зависимости от опер.
            case "+":
                x += y;
                break;
            case "-":
                x -= y;
                break;
            case "*":
                x *= y;
                break;
            case "/":{
                if(y == 0){                     // Обработка деления на ноль.
                    numberField.setText(""); numberField.setHint("Division by zero");
                    X = null; Y = null; previousOperation = null;
                    DivisionByZero = true;
                    OperStatus = false;
                    return;
                }
                x /= y;
                break;
            }
            case "^":
                x = pow(x, y);
                break;
            case "=":{
                if(previousOperation == "=")
                    return;
            }
        }
        numberField.setText(String.valueOf(x));     // Вывод результата и
        X = String.valueOf(x);                      // сохранение состояния для
        Y = null;                                   // следующего действия.
        OperStatus = true;
    }
}