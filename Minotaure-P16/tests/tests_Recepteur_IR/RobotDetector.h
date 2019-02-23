#pragma once

#include "Arduino.h"

struct RobotLocation{
  int angle; //from -179 to 180, 0 is ahead
  short proximite;

  String toString(){return String(angle)+", "+String(proximite);}
};

class RobotDetector {
  public:
    static RobotDetector& getInstance(){
      static RobotDetector instance;
      return instance;
    }
    void detect();
    RobotLocation getNearestRobot();
    RobotLocation getFarestRobot();

    RobotDetector(RobotDetector const&)   = delete;
    void operator=(RobotDetector const&)  = delete;

  private:
    RobotDetector();
    static const byte ID_RECEPTEUR = 0x42;
    static const byte ROBOT_POS = 0x04;
    static const short SENSOR_NUMBER = 9;

    byte robot1Pos;
    byte robot2Pos;
  
};

