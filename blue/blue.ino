#include <Servo.h>
#include <IRremote.h>

// 蓝牙串口数据
byte serialA;
// 舵机、开关相关变量
Servo servo_0;
Servo servo_1;
int servo_need = 30;
int servo_reset = 90;
int servo_on = servo_reset + servo_need;
int servo_off = servo_reset - servo_need;
bool switch_1_state = false;
bool switch_2_state = false;
// 按钮
int button_0 = 5;
int button_1 = 6;
// 蜂鸣器
int buzzer_0 = 7;
// 红外
IRrecv irRecv_0(11);
decode_results irRecv_0_results;
// 光敏
int lightSensor_0 = A0;
bool light_mode = false;
uint32_t timer = 0;

void open_switch_1() {
  servo_0.write(servo_on);
  delay(20);
  servo_0.write(servo_on);
  delay(20);
  servo_0.write(servo_on);
  switch_1_state = true;
  delay(200);
  servo_0.write(servo_reset);
  delay(100);
}
void close_switch_1() {
  servo_0.write(servo_off);
  delay(20);
  servo_0.write(servo_off);
  delay(20);
  servo_0.write(servo_off);
  switch_1_state = false;
  delay(200);
  servo_0.write(servo_reset);
  delay(100);
}
void open_switch_2() {
  servo_1.write(servo_off);
  delay(20);
  servo_1.write(servo_off);
  delay(20);
  servo_1.write(servo_off);
  switch_2_state = true;
  delay(200);
  servo_1.write(servo_reset);
  delay(100);
}
void close_switch_2() {
  servo_1.write(servo_on);
  delay(20);
  servo_1.write(servo_on);
  delay(20);
  servo_1.write(servo_on);
  switch_2_state = false;
  delay(200);
  servo_1.write(servo_reset);
  delay(100);
}
void beep() {
  unsigned char i, j;
  for (i = 0; i < 80; i++)
  {
    digitalWrite(buzzer_0, HIGH);
    delay(1);//延时1ms
    digitalWrite(buzzer_0, LOW);
    delay(1);//延时ms
  }
  for (i = 0; i < 100; i++)
  {
    digitalWrite(buzzer_0, HIGH);
    delay(2);
    digitalWrite(buzzer_0, LOW);
    delay(2);
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(button_0, INPUT);
  pinMode(button_1, INPUT);
  pinMode(buzzer_0, OUTPUT);
  irRecv_0.enableIRIn();
  pinMode(lightSensor_0, INPUT);
  servo_0.attach(9);
  servo_1.attach(10);
  servo_0.write(90);
  servo_1.write(90);
}

void loop() {
  // 蓝牙部分
  if (Serial.available() > 0) {
    serialA = Serial.read();
    // Serial.println(serialA);
    switch (serialA) {
      case 0:
        close_switch_1();
        break;
      case 1:
        open_switch_1();
        break;
      case 2:
        close_switch_2();
        break;
      case 3:
        open_switch_2();
        break;
      case 4:
        light_mode = true;
        timer = millis();
        break;
      case 5:
        servo_need += 1;
        break;
      case 6:
        servo_need += -1;
        break;
      default:
        break;
    }
  }
  // 红外部分
  if (irRecv_0.decode( & irRecv_0_results)) {
    // 按键1，幅度+5
    if (irRecv_0_results.value == 0xFFA25D) {
      servo_need += 5;
      beep();
      delay(200);
    }
    // 按键2，幅度-5
    if (irRecv_0_results.value == 0xFF629D) {
      if (servo_need - 5 >= 0) {
        servo_need += -5;
      }
      beep();
      delay(200);
    }
    // 按键3，光敏开
    if (irRecv_0_results.value == 0xFFE21D) {
      beep();
      light_mode = true;
      timer = millis();
      delay(50);
    }
    // 按键4，光敏关
    if (irRecv_0_results.value == 0xFF22DD) {
      beep();
      light_mode = false;
      timer = millis();
      delay(50);
    }
    // 按键上，开关1开灯
    if (irRecv_0_results.value == 0xFF18E7) {
      beep();
      open_switch_1();
      delay(200);
    }
    // 按键下，开关1关灯
    if (irRecv_0_results.value == 0xFF4AB5) {
      beep();
      close_switch_1();
      delay(200);
    }
    // 按键左，开关2开灯
    if (irRecv_0_results.value == 0xFF10EF) {
      beep();
      open_switch_2();
      delay(200);
    }
    // 按键右，开关2关灯
    if (irRecv_0_results.value == 0xFF5AA5) {
      beep();
      close_switch_2();
      delay(200);
    }
    irRecv_0.resume();
  }
  // 按钮1
  if (digitalRead(button_0) == HIGH) {
    // beep();
    if (switch_1_state == true) {
      close_switch_1();
      delay(500);
    } else {
      open_switch_1();
      delay(500);
    }
  }
  // 按钮2
  if (digitalRead(button_1) == HIGH) {
    if (switch_2_state == true) {
      close_switch_2();
      delay(500);
    } else {
      open_switch_2();
      delay(500);
    }
  }
  // 光敏
  if (light_mode) {
    if ((millis() - timer) >= 10000 && analogRead(A0) > 1000) {
      open_switch_1();
      open_switch_2();
      light_mode = false;
      delay(200);
    }
  }
  servo_on = servo_reset + servo_need;
  servo_off = servo_reset - servo_need;
}
