#include "asservissement.h"
#include "deplacement.h"
#include "servos.h"
#include "angle.h"
#include "strategie_robot.h"
#include "moteurs.h"
#include "ecran.h"
#include "RobotDetector.h"

#define ROBOT_PRINCIPAL 0
#define ROBOT_SECONDAIRE 1
#define ROBOT_TEST 2

int const TEMPS_PARTIE = 100;

//Variables de mesure du temps
long temps=0;

int idAction  = 0;

int const typeRobot = ROBOT_PRINCIPAL;
//int const typeRobot = ROBOT_TEST;

long debut = 0;

bool aDemarre = false;
int cotePin = 10;
bool coteVert = false;

double xInit = 0;
double yInit = 0;
Angle thetaInit = Angle(0);

bool ancienCoteVert = !coteVert;
int pinStart = 53;

int endStopDroit = 49;
int endStopGauche = 51;

int rs = 40, en = 42, d4 = 32, d5 = 34, d6 = 36, d7 = 38;

Deplacement deplace(coteVert, xInit, yInit, thetaInit, typeRobot);

StrategieRobot strat;

long tempsPrecedent;

void setup()
{
  //Initialisation des pins
  pinMode(cotePin, INPUT);
  digitalWrite(cotePin, HIGH);
  pinMode(pinStart,INPUT);
  digitalWrite(pinStart, HIGH);

  pinMode(endStopDroit, INPUT);
  digitalWrite(endStopDroit, HIGH);
  pinMode(endStopGauche, INPUT);
  digitalWrite(endStopGauche, HIGH);


  Serial.begin(1000000);
  Serial.println("Je suis un super mega Robot !");

  Serial3.begin(115200);
  Serial3.println("Je suis un robot bluetooth !");

  switch(typeRobot)
  {
    case ROBOT_PRINCIPAL :
        xInit = 0.17;
        yInit = 0.06;
        thetaInit = Angle(PI/2);
        cotePin = 10;
        pinStart = 53;
        endStopDroit = 49;
        endStopGauche = 51;
        rs = 40;
        en = 42;
        d4 = 32;
        d5 = 34;
        d6 = 36;
        d7 = 38;
      break;

    case ROBOT_SECONDAIRE :
      xInit = 0.505;
      yInit = 0.095;
      thetaInit = Angle(PI/2);
      cotePin = 10;
      pinStart = 53;
      endStopDroit = 49;
      endStopGauche = 51;
      rs = 40;
      en = 42;
      d4 = 32;
      d5 = 34;
      d6 = 36;
      d7 = 38;
      break;

    default:
      xInit = 0;
      yInit = 0;
      thetaInit = Angle(0);
  }

  Ecran::getInstance().init(rs, en, d4, d5, d6, d7);

  Ecran::getInstance().afficherTexte("Initialisation du robot");

  deplace.setCoteVert(coteVert,xInit,yInit,thetaInit, endStopGauche, endStopDroit);
  deplace.initServos();

  strat.positionInitiale(deplace,typeRobot);

  Serial.println("Fin init");
}

//Boucle principale
void loop()
{
  if(!aDemarre)
  {
    coteVert = digitalRead(cotePin) == HIGH;
    //coteVert = false;
    if(coteVert != ancienCoteVert)
    {
      deplace.setCoteVert(coteVert,xInit,yInit,thetaInit, endStopGauche, endStopDroit);
      ancienCoteVert = coteVert;
      if(coteVert)
      {
        Ecran::getInstance().afficherTexte("Cote vert");
      }
      else
      {
        Ecran::getInstance().afficherTexte("Cote orange");
      }
    }

    RobotDetector::getInstance().calibrer();

    aDemarre = digitalRead(pinStart) == HIGH;
    if(aDemarre)
    {
      deplace.setCoteVert(coteVert,xInit,yInit,thetaInit, endStopGauche, endStopDroit);
      debut = micros();
      Ecran::getInstance().afficherTexte("Debut du match");
      RobotDetector::getInstance().finCalibration();
      strat.strategie_robot(deplace,typeRobot);
    }
  }
  else
  {
    if(micros() - debut > TEMPS_PARTIE * 1000000)
    {
      //Fin de la partie
      Moteurs::stopper();
      deplace.arreterTousServos();
      while(1)
      {
        delay(1000);
      }
    }
    deplace.actualiser();

    if(micros() - debut > (tempsPrecedent+1) * 1000000)
    {
      tempsPrecedent = (micros() - debut)/1000000;
      Serial3.println("Temps " + String(tempsPrecedent));
    }
  }

  //On dort pendant 0.01 s
  delay(20);
}
