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
      myservo.write(70);
      delay(400);
      myservo.write(90);
    } if (results.value == 0xFF629D) {
      myservo.write(90);
    } if (results.value == 0xFFE21D) {
      myservo.write(110);
      delay(400);
      myservo.write(90);
    }
    irrecv.resume(); // Receive the next value
  }
  delay(100);
}
