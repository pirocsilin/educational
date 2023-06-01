using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CursorCTRL : MonoBehaviour
{
    // ����������� ���� �� X � Y;
    private float mouseX;
    private float mouseY;

    // �� ������� joystick ����� �������� �������� ����������� ��������
    public Joystick joystick;

    // ������ Player ����� �������� ����� �������� ��� ���������
    public Transform Player;

    // ������ ���������������� ����
    public float sensMouse = 140f;

    void Update()
    {
        // ���� �� ����� ����, ���������� ������
        if(!FirstFaceMove.End) GetMobileInput();
    }

    private void GetMobileInput(){

        mouseX = joystick.Horizontal * sensMouse * Time.deltaTime;
        mouseY = joystick.Vertical * sensMouse * Time.deltaTime;
        
        // �������� �������� ����������� ���� �� ���� X � Y (������� � ������ ������)
        // mouseX = Input.GetAxis("Mouse X") * sensMouse * Time.deltaTime;
        // mouseY = Input.GetAxis("Mouse Y") * sensMouse * Time.deltaTime;

        // ������� ������ ������ ��� Y ��� �������� ���� �� ��� X
        if(joystick.Horizontal >= 0.1f || joystick.Horizontal <= -0.1f)
            Player.Rotate(mouseX * new Vector3(0,1,0));

        // ������� ������. ���������� �������� � Transform ��� ��� ������ �� ������
        // transform.Rotate(-mouseY * new Vector3(1,0,0));
    }
}
