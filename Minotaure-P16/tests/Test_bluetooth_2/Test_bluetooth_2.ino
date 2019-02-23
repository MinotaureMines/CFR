char myChar;

void setup() {
  // put your setup code here, to run once:
  Serial3.begin(115200);
  Serial.begin(1000000);
}

void loop() {
  
  while(Serial3.available()){
    myChar = Serial3.read();
    Serial3.print("\nreception : ");
    Serial3.print(myChar);
  }

  while(Serial.available()){
    myChar = Serial.read();
    Serial.print(myChar);
    Serial3.print(myChar);
  }

}
