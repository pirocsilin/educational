
using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;

public class FirstFaceMove : MonoBehaviour
{
    // Из объекта joystick будем получать значения перемещения джостика
    public Joystick joystick;
    // Переменная _mainLight для управления светом
    public Light _mainLight;

    public Transform[] obgects = new Transform[9];

    public GameObject restart;

    public int Timer = 30;

    public static bool End;

    public float speed = 6f, rspeed = 50f;
    public Text scoreText;
    public Text restartButton;
    private Rigidbody _rb;
    private int score = 0;

    private float vertical;
    private float horizontal;

    private void Start() {

        for(int i=0; i<obgects.Length; i++){
            obgects[i].Translate(new Vector3(RandNum(),0,RandNum()));
        }
        End = false;
        StartCoroutine(GameTimer());
    }

    private void Update() {

        // Пока не конец игры, перемещаем игрока
        if(transform.localPosition.y <= -1) 
            End = true;

        if(!End){
            GetMobileInput();
            RotateObgects();
        }
    }

    private IEnumerator GameTimer(){
        
        while(Timer >= 0 && !End){
            yield return new WaitForSeconds(1f);
            if(End) break;
            scoreText.text = "Time: " + --Timer + " | Score: " + score;
        }
        if(!End){
            End = true;
            scoreText.text = "You Lose";
            _mainLight.enabled = !_mainLight.enabled;
            restart.SetActive(true);
            restartButton.text = "RESTART GAME";
        }
    }

    private float RandNum(){
        return UnityEngine.Random.Range(-8.5f, 8.5f);
    }

    private void RotateObgects(){
        for(int i=0; i<obgects.Length; i++){
            if(obgects[i] == null)              // Если объект еще существует, то
                continue;                       // поворачиваем его иначе пропуск
            obgects[i].Rotate(new Vector3(0,1,0) * rspeed * Time.deltaTime);
        }
    }

    private void GetMobileInput(){

        // Получаем значение при меремещении джойстика
        vertical = joystick.Vertical;
        horizontal = joystick.Horizontal;
            
        //if(Input.GetKey(KeyCode.W))
        if(vertical >= 0.2f){
            transform.localPosition += transform.forward * speed * Time.deltaTime;
        }
        //if(Input.GetKey(KeyCode.S))
        if(vertical <= -0.2f){
            transform.localPosition += -transform.forward * speed * Time.deltaTime;
        }
        //if(Input.GetKey(KeyCode.A))
        if(horizontal <= -0.2f){
            transform.localPosition += -transform.right * speed * Time.deltaTime;
        }
        //if(Input.GetKey(KeyCode.D))
        if(horizontal >= 0.2f){
            transform.localPosition += transform.right * speed * Time.deltaTime;
        }
    }
    
    // Если мы коснулись объекта с тегом CrashTag, то удаляем этот объект
    private void OnTriggerEnter(Collider other) {
        if(other.gameObject.tag == "CrashTag"){
            score ++;
            Destroy(other.gameObject);
            if(score < obgects.Length)
                scoreText.text = "Time: " + Timer + " | Score: " + score;
            if(score == obgects.Length){
                End = true;
                scoreText.text = "You Win";
                _mainLight.enabled = !_mainLight.enabled;
                restart.SetActive(true);
                restartButton.text = "RESTART GAME";
            }
        }
    }
}
