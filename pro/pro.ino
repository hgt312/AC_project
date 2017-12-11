/**
 * Copyright(C), 2016-2038, KenRobot.com
 * FileName: pro.ino
 * Author: 啃萝卜
 * Create: 2017/12/11
 * Modify: 2017/12/11
 */

#include <IRremote.h>
#include <Servo.h>
#include <SoftwareSerial.h>
 
int button_0 = 5;
int button_1 = 6;
int buzzer_0 = 7;
IRrecv irRecv_0(11);
decode_results irRecv_0_results;
int lightSensor_0 = A0;
Servo servo_0;
Servo servo_1;
SoftwareSerial wifi_0(0, 1);
int servo_on = 100;
int servo_off = 80;
int servo_need = 10;
bool switch_1_state = false;
bool switch_2_state = false;
bool light_mode = false;
uint32_t timer = 0;
void open_switch_1() {
    servo_0.write((servo_on + servo_need));
    delay(100);
    servo_0.write(servo_on);
    delay(100);
    servo_0.write(servo_on);
    switch_1_state = true;
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
}
void open_switch_2() {
    servo_1.write((servo_on + servo_need));
    delay(100);
    servo_1.write(servo_on);
    delay(100);
    servo_1.write(servo_on);
    switch_2_state = true;
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
}

void setup() {
    pinMode(button_0, INPUT);
    pinMode(button_1, INPUT);
    pinMode(buzzer_0, OUTPUT);
    irRecv_0.enableIRIn();
    pinMode(lightSensor_0, INPUT);
    servo_0.attach(9);
    servo_1.attach(10);
    wifi_0.begin(9600);
    servo_0.write(servo_off);
    servo_1.write(servo_off);
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
            servo_0.write(servo_0.read() + 5);
            servo_1.write(servo_1.read() + 5);
            delay(500);
        }
        /*
        按键下
        */
        if (irRecv_0_results.value == 0xFF4AB5) {
            servo_0.write(servo_0.read() - 5);
            servo_1.write(servo_1.read() - 5);
            delay(500);
        }
        /*
        按键3光敏开
        */
        if (irRecv_0_results.value == 0xFFE21D) {
            switch_1_state = true;
            timer = millis();
            delay(50);
        }
        /*
        按键4光敏关
        */
        if (irRecv_0_results.value == 0xFF22DD) {
            switch_1_state = false;
            timer = millis();
            delay(50);
        }
        irRecv_0.resume();
    }
    delay(50);
    /*
    按钮1
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