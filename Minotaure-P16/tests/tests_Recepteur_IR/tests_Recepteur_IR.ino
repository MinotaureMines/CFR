
#include "RobotDetector.h"

void setup() {
         // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  
}

void loop() {
  RobotDetector::getInstance().detect();
  RobotLocation tmpLoc = RobotDetector::getInstance().getNearestRobot();
  Serial.println(tmpLoc.toString());
  delay(500);
}
