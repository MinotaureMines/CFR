#include <Servo.h>
String readString, servoVal;
Servo servo;
int n1 =0;

//Il y a une légère hystérésis: le moteur se met en route pour 64 mais s'éteint pour 62

void setup(){
  Serial.begin(9600);
  servo.attach(9);
}
void loop(){

  while (Serial.available()) {
    delay(3);  //delay to allow buffer to fill
    if (Serial.available() >0) {
      char c = Serial.read();  //gets one byte from serial buffer
      readString += c; //makes the string readString
    }
  }

  if (readString.length() >0) {
      Serial.println(readString); //see what was received
     
      // expect a string like 07002100 containing the two servo positions     
      servoVal = readString.substring(0, 3); //get the first three characters

     
      Serial.println(servoVal);  //print to serial monitor to see parsed results

      n1 = servoVal.toInt();

      Serial.println("the numbers are :");
      Serial.println(n1);  //print to serial monitor to see number results
      readString="";
  }
  servo.write(n1);
  Serial.println(n1);
  delay(100);
}

