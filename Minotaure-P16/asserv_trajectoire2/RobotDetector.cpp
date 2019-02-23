#include "RobotDetector.h"

//340 /600
//RobotDetector::SensorData RobotDetector::sensorData[] = {{0.13, Angle(-0.175), A3, 0.}, {0.13, Angle(0.175), A4, 0}};//, {0.13, 400, Angle(3.14), A2, 0.}};
RobotDetector::SensorData RobotDetector::sensorData[] = {{0.13, Angle(-0.175), A3, 0.}, {0.13, Angle(0.175), A4, 0}}; //, {0.13, 600, Angle(3.14), A2, 0.}};


RobotDetector::RobotDetector(){
    for (auto data : sensorData){
        pinMode(data.port, INPUT);   
    }
}

RobotLocation RobotDetector::detect(double robotX, double robotY, Angle robotAngle){
  int value;
  double mesuredDistances[SENSOR_NUMBER];
  for(int i=0; i < SENSOR_NUMBER; i++){
    value = analogRead(sensorData[i].port);
    double dtsTau = (micros()-prevT)/tau;
    sensorData[i].moyenneGlissante = sensorData[i].moyenneGlissante*max(1-dtsTau, 0) + min(dtsTau, 1)*value;
    if(sensorData[i].moyenneGlissante>=seuil){
        mesuredDistances[i] = sensorData[i].activationDistance;
        Serial3.print("oeil ");
        Serial3.print(i);
        Serial3.print(" ");
        Serial3.println(sensorData[i].moyenneGlissante);
    }else{
        mesuredDistances[i] = 0;
    }
  }

  prevT = micros();
  RobotLocation res = {Angle(0), 0.};
  int nbDetected = 0;
  for(int i=0; i< SENSOR_NUMBER; i++){
    //Serial3.println(mesuredDistances[i]);
      if(mesuredDistances[i] != 0){
          nbDetected ++;
          res.angle += sensorData[i].sensorAngle;
          res.distance += mesuredDistances[i];
      }
  }
  if(nbDetected == 0 || deactivateOnEdges(robotX, robotY, robotAngle)){
      return {Angle(0), 10.};
  }else{
      res.angle /= nbDetected;
      res.distance /= nbDetected;
      return res;
  }
}

void RobotDetector::calibrer(){
  int mesure;
  for(int i=0; i< SENSOR_NUMBER; i++){
    mesure = analogRead(sensorData[i].port);
    if(mesure<calibrationMin){
      calibrationMin = mesure;
    }else if(mesure>calibrationMax){
      calibrationMax = mesure;
    }
  }
}

void RobotDetector::finCalibration(){
  if(calibrationMax - calibrationMin < 150){
    Serial3.println("Calibration annulée");
    seuil = 600;
  }else{  
    seuil = calibrationMin + (calibrationMax - calibrationMin)*.9;
    Serial3.print("Calibration a ");
    Serial3.println(seuil);
  }
}

bool RobotDetector::deactivateOnEdges(double robotX, double robotY, Angle robotAngle){
  if(robotAngle <= PI/4 && robotAngle >= -PI/4 && robotX >= 1.8){
      return true;
  }else if(robotAngle >= PI/4 && robotAngle <= 3*PI/4 && robotY >= 2.7){
    return true;
  }else if(robotAngle <= -PI/4 && robotAngle >= -3*PI/4 && robotY <= 0.3){
    return true;
  }else if(robotAngle >= -PI/4 && robotAngle <= PI/4 && robotX >= 1.7){
    return true;
  }else if((robotAngle >= 3*PI/4 || robotAngle <= -3*PI/4) && robotX <= 0.3){
    return true;
  }else{
    return false;
  }
}

