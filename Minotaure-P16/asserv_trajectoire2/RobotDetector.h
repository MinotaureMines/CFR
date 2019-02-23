#ifndef ROBOT_DETECTOR_H
#define ROBOT_DETECTOR_H

#include "Arduino.h"
#include "angle.h"

struct RobotLocation{
  Angle angle; //from -179 to 180, 0 is ahead
  double distance;

  String toString(){return String(angle.versFloat())+", "+String(distance);}
};

class RobotDetector {
  public:
    static RobotDetector& getInstance(){
      static RobotDetector instance;
      return instance;
    }


    RobotDetector(RobotDetector const&)   = delete;
    void operator=(RobotDetector const&)  = delete;

    RobotLocation detect(double robotX, double robotY, Angle robotAngle);

    void calibrer();

    void finCalibration();

  private:
    RobotDetector();
    static bool deactivateOnEdges(double robotX, double robotY, Angle robotAngle);

public:

  static constexpr double MINIMUM_DISTANCE = 0.15;
  static constexpr double DISTANCE_DETECTION = 0.3;
private:
    struct SensorData{
        double activationDistance;
        Angle sensorAngle;
        short port;
        double moyenneGlissante;
    };
    //2 capteurs à -10°, 0 et +10°
    static constexpr int SENSOR_NUMBER = 2;
    static SensorData sensorData[SENSOR_NUMBER];
    static constexpr int tau = 5000;
    int calibrationMin = 200;
    int calibrationMax = 500;
    int seuil = 500;
    long prevT = 0;
};

#endif
