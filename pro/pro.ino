/**
 * Copyright(C), 2016-2038, KenRobot.com
 * FileName: pro.ino
 * Author: 啃萝卜
 * Create: 2017/12/11
 * Modify: 2017/12/11
 */

#include <IRremote.h>
#include <Servo.h>
 
int button_0 = 5;
int button_1 = 6;
IRrecv irRecv_0(11);
decode_results irRecv_0_results;
int lightSensor_0 = A0;
Servo servo_0;
Servo servo_1;
int servo_on = 100;
int servo_off = 80;
int servo_need = 10;
bool switch_1_state = false;
bool switch_2_state = false;
void open_switch_1() {
    servo_0.write((servo_on + servo_need));
    delay(400);
    servo_0.write(servo_on);
    delay(400);
    button_0.write(servo_on);
    switch_1_state = true;
    delay(400);
}
void close_switch_1() {
    servo_0.write((servo_off - servo_need));
    delay(400);
    servo_0.write(servo_off);
    delay(400);
    button_0.write(servo_off);
    switch_1_state = false;
    delay(400);
}
void open_switch_2() {
    servo_1.write((servo_on + servo_need));
    delay(400);
    servo_1.write(servo_on);
    delay(400);
    servo_1.write(servo_on);
    switch_2_state = true;
    delay(400);
}
void close_switch_2() {
    servo_1.write((servo_off - servo_need));
    delay(400);
    servo_1.write(servo_off);
    delay(400);
    servo_1.write(servo_off);
    switch_2_state = false;
    delay(400);
}

void setup() {
    pinMode(button_0, INPUT);
    pinMode(button_1, INPUT);
    irRecv_0.enableIRIn();
    pinMode(lightSensor_0, INPUT);
    servo_0.attach(9);
    servo_1.attach(10);
    Serial.begin(9600);
    servo_0.write(servo_off);
    servo_1.write(servo_off);
}

void loop() {
    /*
    红外部分
    */
    if (irRecv_0.decode( & irRecv_0_results)) {
        /*
        按键1，打开开关1
        */
        if (irRecv_0_results.value == 0xFFA25D) {
            open_switch_1();
        }
        /*
        按键2，关闭开关1
        */
        if (irRecv_0_results.value == 0xFF629D) {
            close_switch_1();
        }
        /*
        按键4，打开开关2
        */
        if (irRecv_0_results.value == 0xFF22DD) {
            open_switch_2();
        }
        /*
        按键5，关闭开关1
        */
        if (irRecv_0_results.value == 0xFF02FD) {
            close_switch_2();
        }
        irRecv_0.resume();
    }
    delay(100);
    /*
    按钮部分
    */
    if (digitalRead(button_0) == HIGH) {
        if (switch_1_state == true) {
            close_switch_1();
            delay(500);
        } else {
            open_switch_1();
            delay(500);
        }
    }
    if (digitalRead(button_1) == HIGH) {
        if (switch_2_state == true) {
            close_switch_2();
            delay(500);
        } else {
            open_switch_2();
            delay(500);
        }
    }
}