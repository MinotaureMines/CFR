#include "RobotDetector.h"

#include <Wire.h>

RobotDetector::RobotDetector(){
  Wire.begin();
}

void RobotDetector::detect(){
  Wire.beginTransmission(ID_RECEPTEUR);
  Wire.write(ROBOT_POS);
  Wire.requestFrom(ID_RECEPTEUR, (byte)2);    // read ROBOT1_POS and ROBOT2_POS
  
  robot1Pos = Wire.read();
  robot2Pos = Wire.read();
  Wire.endTransmission();
}

RobotLocation RobotDetector::getNearestRobot(){
    int angle = (int)((robot1Pos&0x1f )/(2.f * SENSOR_NUMBER) *360 + 180)%360 - 180;
    return {angle, robot1Pos>>5};
}
RobotLocation RobotDetector::getFarestRobot(){
  int angle = (int)((robot2Pos&0x1f )/(2.f * SENSOR_NUMBER) *360 + 180)%360 - 180;
    return {angle, robot2Pos>>5};
}

