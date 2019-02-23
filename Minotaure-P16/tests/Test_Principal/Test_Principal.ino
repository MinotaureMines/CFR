#include "moteurs.h";

void setup() {
  // put your setup code here, to run once:
  Moteurs::init();
}

void loop() {
  // put your main code here, to run repeatedly:
  Moteurs::avancent(0, -200);
  delay(1000);
}
