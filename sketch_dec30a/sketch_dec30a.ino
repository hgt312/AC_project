#include <Servo.h>
#include <IRremote.h>

// 蓝牙串口数据
byte serialA;
// 舵机、开关相关变量
Servo servo_0;
Servo servo_1;
int servo_on = 100;
int servo_off = 80;
int servo_need = 10;
int servo_reset = 90;
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
  servo_0.write(servo_off);
  servo_1.write(servo_off);
}

void loop() {
  beep();
  delay(1000);
}
