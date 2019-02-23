#ifndef SERVOS_H
#define SERVOS_H

#include <DynamixelMotor.h>

#include "angle.h"

#define SERIAL_SERVOS Serial2

// Les numéros des servos
enum Servo{
    Servo1 = 1,
    Servo2 = 6,
    Servo3 = 12,
    Servo4 = 16
};

class Servos{
public:
    static Servos& getInstance();
    void setAngle(Servo servo, Angle &angle);

    Servos(Servos const&) = delete;
    void operator=(Servos const&) = delete;

private:
    explicit Servos();
    String octetComplet(String chaine);
    HardwareDynamixelInterface mInterface;

public:
    const long unsigned int BAUDRATE = 57142;
    const float PLAGE_SERVOS = 5.236f;
};



#endif
