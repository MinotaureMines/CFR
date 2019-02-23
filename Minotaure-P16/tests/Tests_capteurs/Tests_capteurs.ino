//double moy = 0;
//const double tau = 5000;
//long prevT = 0;
//
//void setup() {
//  // put your setup code here, to run once:
//  Serial3.begin(115200); 
//  Serial.println("test start");
//  pinMode(A2, INPUT);
//  
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  int a = analogRead(A3);
//  double dtsTau = (micros()-prevT)/tau;
//  prevT = micros();
//  moy = moy*max(1-dtsTau, 0) + min(dtsTau, 1)*a;
// /// Serial.print("dtsTau: ");
// // Serial.println(dtsTau);
//  //moy = moy*exp(-dtsTau) + a*(1-exp(-dtsTau));
//  Serial3.println(moy);
//  delay(1); 
//}
double moy = 0;
const double tau = 10000;
long prevT = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); 
  Serial.println("test start");
  pinMode(A4, INPUT);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int a = analogRead(A4);
  double dtsTau = (micros()-prevT)/tau;
  prevT = micros();
  moy = moy*max(1-dtsTau, 0) + min(dtsTau, 1)*a;
 /// Serial.print("dtsTau: ");
 // Serial.println(dtsTau);
  //moy = moy*exp(-dtsTau) + a*(1-exp(-dtsTau));
  Serial.println(moy);
  delay(1); 
}
