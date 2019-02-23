char myChar;

void setup() {
  // put your setup code here, to run once:
  Serial3.begin(38400);
  Serial.begin(1000000);

  Serial.println("AT");
  Serial3.println("AT");
}

void loop() {
  
  while(Serial3.available()){
    myChar = Serial3.read();
    Serial.print(myChar);
  }

  while(Serial.available()){
    myChar = Serial.read();
    Serial.print(myChar);
    Serial3.print(myChar);
  }

}
