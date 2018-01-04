
/**
 * Copyright(C), 2016-2038, KenRobot.com
 * FileName: pro.ino
 * Author: 啃萝卜
 * Create: 2017/12/11
 * Modify: 2017/12/11
 */

#include <IRremote.h>
#include <Servo.h>
#include <MsTimer2.h>  

int button_0 = 5;
int button_1 = 6;
int buzzer_0 = 7;
IRrecv irRecv_0(11);
decode_results irRecv_0_results;
int lightSensor_0 = A0;
Servo servo_0;
Servo servo_1;
int servo_on = 100;
int servo_off = 80;
int servo_need = 10;
int servo_reset = 90;
bool switch_1_state = false;
bool switch_2_state = false;
bool light_mode = false;
uint32_t timer = 0;

int  intervalometer = 10000;
bool timer_mode_on = false;
bool timer_mode_off = false;

void beep(){   
unsigned char i,j;
        for(i=0;i<80;i++)
        {
          digitalWrite(buzzer_0,HIGH);
          delay(1);//延时1ms
          digitalWrite(buzzer_0,LOW);
          delay(1);//延时ms
        }
        for(i=0;i<100;i++)
        {
          digitalWrite(buzzer_0,HIGH);
          delay(2);
          digitalWrite(buzzer_0,LOW);
          delay(2);
        }
}



void open_switch_1() {
    servo_0.write((servo_on + servo_need));
    delay(100);
    servo_0.write(servo_on);
    delay(100);
    servo_0.write(servo_on);
    switch_1_state = true;
    delay(100);
    servo_0.write(servo_reset);
    delay(100);
}
void close_switch_1() {
    servo_0.write((servo_off - servo_need));
    delay(100);
    servo_0.write(servo_off);
    delay(100);
    servo_0.write(servo_off);
    switch_1_state = false;
    delay(100);
    servo_0.write(servo_reset);
    delay(100);
}
void open_switch_2() {
    servo_1.write((servo_on + servo_need));
    delay(100);
    servo_1.write(servo_on);
    delay(100);
    servo_1.write(servo_on);
    switch_2_state = true;
    delay(100);
    servo_1.write(servo_reset);
    delay(100);
}
void close_switch_2() {
    servo_1.write((servo_off - servo_need));
    delay(100);
    servo_1.write(servo_off);
    delay(100);
    servo_1.write(servo_off);
    switch_2_state = false;
    delay(100);
    servo_1.write(servo_reset);
    delay(100);
}

void TIMER(){
  if(timer_mode_on){
    open_switch_1();
    }

    else if(timer_mode_off){
   close_switch_1();
    }
  }

void setup() {
    pinMode(button_0, INPUT);
    pinMode(button_1, INPUT);
    pinMode(buzzer_0, OUTPUT);
    irRecv_0.enableIRIn();
    pinMode(lightSensor_0, INPUT);
    servo_0.attach(9);
    servo_1.attach(10);
    servo_0.write(servo_off);
    servo_1.write(servo_off);

 // MsTimer2::set(intervalometer, open_switch_1);
 
}

void loop() {
    /*
    红外部分
    */
    if (irRecv_0.decode( & irRecv_0_results)) {
        /*
        按键1
        */
        if (irRecv_0_results.value == 0xFFA25D) {
          beep();
            if (switch_1_state == true) {
                close_switch_1();
                delay(500);
            } else {
                open_switch_1();
                delay(500);
            }
        }
        /*
        按键2
        */
        if (irRecv_0_results.value == 0xFF629D) {
          beep();
            if (switch_1_state == true) {
                close_switch_2();
                delay(500);
            } else {
                open_switch_2();
                delay(500);
            }
        }
        /*
        按键上
        */
        if (irRecv_0_results.value == 0xFF18E7) {
          beep();
            servo_0.write(servo_0.read() + 5);
            servo_1.write(servo_1.read() + 5);
            delay(500);
        }
        /*
        按键下
        */
        if (irRecv_0_results.value == 0xFF4AB5) {
          beep();
            servo_0.write(servo_0.read() - 5);
            servo_1.write(servo_1.read() - 5);
            delay(500);
        }
        /*
        按键3光敏开
        */
        if (irRecv_0_results.value == 0xFFE21D) {
          beep();
            switch_1_state = true;
            timer = millis();
            delay(50);
        }
        /*
        按键4光敏关
        */
        if (irRecv_0_results.value == 0xFF22DD) {
          beep();
            switch_1_state = false;
            timer = millis();
            delay(50);
        }
        /*
        按键5 开启定时开灯模式
        */
        if (irRecv_0_results.value == 0xFF02FD) {
          beep();
            timer_mode_on = true;
            intervalometer = 0;
            
        }
        /*
        按键6 开启定时关灯模式
        */
        if (irRecv_0_results.value == 0xFFC23D) {
             beep();
            timer_mode_off = true;
            intervalometer = 0;
        }
        /*
        按键7 取消定时模式 
        */
        if (irRecv_0_results.value == 0xFFE01F) {
            beep();
            timer_mode_on = false;
            timer_mode_off = false;
         //   MsTimer2::stop( );
        }
        /*
        按键8 按5/6按键后每按一次设置时间+5s
        */
        if (irRecv_0_results.value == 0xFFA857) {
          beep();
          if(timer_mode_on||timer_mode_off){
            intervalometer += 5000;}
        }
          /*
        按键9 按5/6按键后每按一次设置时间+10s
        */
        if (irRecv_0_results.value == 0xFF906F) {
            beep();
             if(timer_mode_on||timer_mode_off){
            intervalometer += 10000;}
        }
        /*
        按键0 按5/6按键后每按一次设置时间+20s
        */
        if (irRecv_0_results.value == 0xFF9867) {
            beep();
             if(timer_mode_on||timer_mode_off){
            intervalometer += 20000;}
        }
        /*
        按键OK 完成设定时间操作
        */
         if (irRecv_0_results.value == 0xFF38C7) {
            beep();
            
          //  MsTimer2::start( );
        }
        irRecv_0.resume();
        
    }
    delay(50);
    /*
    按钮1
    */
    if (digitalRead(button_0) == HIGH) {
      beep();
        if (switch_1_state == true) {
            close_switch_1();
            delay(500);
        } else {
            open_switch_1();
            delay(500);
        }
    }
    /*
    按钮2
    */
    if (digitalRead(button_1) == HIGH) {
        if (switch_2_state == true) {
            close_switch_2();
            delay(500);
        } else {
            open_switch_2();
            delay(500);
        }
    }
    /*
    光敏模式
    */
    if (light_mode) {
        if ((millis() - timer) >= 10000) {
            close_switch_1();
            close_switch_2();
            delay(500);
        }
    }
}
