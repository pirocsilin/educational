using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CursorCTRL : MonoBehaviour
{
    // Перемещение мыши по X и Y;
    private float mouseX;
    private float mouseY;

    // Из объекта joystick будем получать значения перемещения джостика
    public Joystick joystick;

    // Объект Player будет получать новые значения для разворота
    public Transform Player;

    // Задаем чувтсвительность мыши
    public float sensMouse = 140f;

    void Update()
    {
        // Пока не конец игры, перемещаем игрока
        if(!FirstFaceMove.End) GetMobileInput();
    }

    private void GetMobileInput(){

        mouseX = joystick.Horizontal * sensMouse * Time.deltaTime;
        mouseY = joystick.Vertical * sensMouse * Time.deltaTime;
        
        // Получаем значения перемещения мыши по осям X и Y (Поворот и наклон игрока)
        // mouseX = Input.GetAxis("Mouse X") * sensMouse * Time.deltaTime;
        // mouseY = Input.GetAxis("Mouse Y") * sensMouse * Time.deltaTime;

        // Поворот игрока вокруг оси Y при движении мыши по оси X
        if(joystick.Horizontal >= 0.1f || joystick.Horizontal <= -0.1f)
            Player.Rotate(mouseX * new Vector3(0,1,0));

        // Поворот камеры. Обращаемся напрямую к Transform так как скрипт на камере
        // transform.Rotate(-mouseY * new Vector3(1,0,0));
    }
}
