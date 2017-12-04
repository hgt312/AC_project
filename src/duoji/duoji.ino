#include <Servo.h>
#include <IRremote.h>

int RECV_PIN = 11;

Servo myservo;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  myservo.attach(10);
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
}

void loop()
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0xFFA25D) {
      myservo.write(45);
      // delay(1000);
      // myservo.write(0);
    } if (results.value == 0xFF629D) {
      myservo.write(90);
    } if (results.value == 0xFFE21D) {
      myservo.write(135);
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
