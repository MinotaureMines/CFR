#include "servos.h"
#include "ecran.h"

Servos::Servos():
mInterface(SERIAL_SERVOS)
{
	//Ouverture de l'interface de communication
    mInterface.begin(BAUDRATE);
	
	// Gestion de la vitesse des servos (par défaut, ils sont à vitesse maximale)
    mInterface.write(Servo::Servo1, DYN_ADDRESS_GOAL_SPEED, (unsigned int)200);
    mInterface.write(Servo::Servo2, DYN_ADDRESS_GOAL_SPEED, (unsigned int)200);
    mInterface.write(Servo::Servo3, DYN_ADDRESS_GOAL_SPEED, (unsigned int)200);
    mInterface.write(Servo::Servo4, DYN_ADDRESS_GOAL_SPEED, (unsigned int)200);
}

Servos& Servos::getInstance(){
    static Servos instance;

    return instance;
}

String Servos::octetComplet(String chaine)
{
  String prov;
  while(chaine.length() < 8)
  {
    prov = "0";
    prov.concat(chaine);
    chaine = prov;
  }
  return chaine;
}

void Servos::setAngle(Servo servo, Angle &angle)
{
  mInterface.write(servo, DYN_ADDRESS_GOAL_POSITION, (int)(angle.versFloatPositif()/PLAGE_SERVOS*1023));

/*
  uint8_t erreur;
  mInterface.read(servo, 0x12, erreur);

  Serial3.println("Servo" + String(servo) + " " + octetComplet(String(erreur, BIN)));

  uint8_t tempMax, tempAct;
  mInterface.read(servo, 0x0B, tempMax);
  mInterface.read(servo, 0x2B, tempAct);

  uint8_t chargeMaxLow, chargeMaxHigh, chargeActLow, chargeActHigh;
  
  mInterface.read(servo, 0x28, chargeActLow);
  mInterface.read(servo, 0x29, chargeActHigh);
  mInterface.read(servo, 0x22, chargeMaxLow);
  mInterface.read(servo, 0x23, chargeMaxHigh);

  Serial3.println("Tmax " + octetComplet(String(tempMax, BIN)) + " TAct " + octetComplet(String(tempAct, BIN)));
  Serial3.println("ChargeMax " + octetComplet(String(chargeMaxHigh, BIN)) + " " + octetComplet(String(chargeMaxLow,BIN)) + " ChargeAct " + 
  octetComplet(String(chargeActHigh,BIN)) + " " + octetComplet(String(chargeActLow,BIN)));
  */
}
