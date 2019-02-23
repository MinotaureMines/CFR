#include "commande.h"
#include "asservissement.h"
#include "deplacement.h"
#include "ecran.h"
#include "Arduino.h"

Commande::Commande(){}
Commande::~Commande(){}
void Commande::executer(){}

CommandeMoteur::CommandeMoteur(double iX, double iY, Angle iTheta, bool iAngleImpose, bool iMarcheArriere, ParamTraj& iParams,bool iVerification, Deplacement *iPere)
{
  Serial.println("Creation commande moteur");
  x = iX;
  y = iY;
  theta = iTheta;
  marcheArriere = iMarcheArriere;
  angleImpose = iAngleImpose;
  pere = iPere;
  params = iParams;
  verification = iVerification;
}

CommandeMoteur::~CommandeMoteur(){}

double CommandeMoteur::valeurAbsolue(double initial)
{
  if(initial < 0)
  {
    return -initial;
  }
  else
  {
    return initial;
  }
}
CommandeTournerAX12::CommandeTournerAX12(DynamixelMotor &servo, int nbTours, int16_t vitesse, Angle reference, Deplacement *iPere)
:mServo(servo), _nbTours(nbTours), _vitesse(vitesse), _reference(reference), pere(iPere)
{
  
}
CommandeTournerAX12::~CommandeTournerAX12(){}

void CommandeTournerAX12::executer(){
    Serial.print("On fait tourner les servos de ");
    Serial.println(_nbTours);
    if(_nbTours==0){
      mServo.speed(_vitesse);
      return;
    }
    uint16_t ref = 0;
    if(_reference.versFloat() == 0){
      ref = mServo.currentPosition();
    }else{
      ref = _reference.versFloatPositif()/PLAGE_SERVOS*1023;
    }
    int cpt = 0;
    uint16_t tmp;
    mServo.wheelMode();
    mServo.speed(_vitesse);
    bool detection = false;

    while(cpt<_nbTours){
      tmp = mServo.currentPosition();
      if(_vitesse>0){
        if(!detection && tmp < ref){
          detection = true;
        }else if(detection && tmp >= ref){
          detection = false;
          cpt++;
        }
      }else{
        if(!detection && tmp > ref){
          detection = true;
        }else if(detection && tmp <= ref){
          detection = false;
          cpt++;
        }
      }
      loop();
    }
    Serial.println("fin du tour des servos");
    mServo.speed(0);
}
Angle CommandeMoteur::caculAngle(double xDepart, double xArrivee, double yDepart, double yArrivee)
{
   if(valeurAbsolue(xArrivee - xDepart) < 0.002 && valeurAbsolue(yArrivee - yDepart) < 0.002)
   {
      Serial.println("Pas intéressant");
        return 0;
   }
    double deltax = xArrivee - xDepart;
    double alpha;
    if(deltax==0)
    {
        if(yArrivee < yDepart)
            alpha = 3.0 * PI/2.0;
        else
            alpha =  PI/2.0;
    }
    else if(deltax > 0)
        alpha =  atan( (yArrivee - yDepart)/(xArrivee - xDepart));
    else
        alpha =  M_PI + atan((yArrivee - yDepart)/(xArrivee - xDepart));

    Serial.println(alpha);
    return Angle(alpha);
}

void CommandeMoteur::executer()
{
  Serial.println(y);
  if(!pere->getCoteVert())
  {
    Serial.println("Orange");
    y= 3 - y;
    theta = Angle(0) - theta;
  }
  double xActuel = pere->asser.getX();
  double yActuel = pere->asser.getY();
  Angle thetaActuel = pere->asser.getTheta();
  Angle thetaDeplacement = caculAngle(xActuel,x,yActuel,y);

  Serial.println("x " + String(xActuel) + " -> " + String(x) + "  y " + String(yActuel) + " -> " + String(y));

  pere->setVerification(verification);

  if(angleImpose)
  {
    Serial.println("Angle impose " + String(thetaActuel.versFloat()) + " -> " + String(theta.versFloat()));
    Trajectoire* traj = new Rotation(xActuel, yActuel, thetaActuel, Angle(theta), params.vitesseRotation, params.accRotation);
    pere->setDernierTheta(theta);
    pere->ajouterFileTraj(traj);       
  }
  else
  {
    pere->setDernierX(x);
    pere->setDernierY(y);
         
    if(marcheArriere)
    {
      thetaDeplacement = thetaDeplacement + Angle(PI);
      params.vitesseAvancer = - params.vitesseAvancer;
    }
    
    Trajectoire* traj;
    Serial.println("Theta Actuel " + String(thetaActuel.versFloat()));
    Serial.println("Theta Deplacement " + String(thetaDeplacement.versFloat()));
    if(valeurAbsolue((thetaDeplacement - thetaActuel).versFloat()) > 0.02)
    {
      traj = new Rotation(xActuel, yActuel, thetaActuel, thetaDeplacement, params.vitesseRotation, params.accRotation);
      pere->ajouterFileTraj(traj);
    }
      
    pere->setDernierTheta(thetaDeplacement);
     
    traj = new Droite(xActuel, yActuel, x, y, params.vitesseAvancer, params.accAvancer);
          
    pere->ajouterFileTraj(traj);
  }
}

CommandeAX12::CommandeAX12(DynamixelMotor &servo, Angle &angle, Deplacement *iPere):mAngle(angle), mServo(servo), pere(iPere)
{
}
CommandeAX12::~CommandeAX12(){}

void CommandeAX12::executer()
{
  mServo.goalPosition((int)(mAngle.versFloatPositif()/PLAGE_SERVOS*1023));
}

CommandeAttente::CommandeAttente(float iTemps, Deplacement *iPere)
{
  temps = iTemps;
  pere = iPere;
}

CommandeAttente::~CommandeAttente()
{
  
}

void CommandeAttente::executer()
{
  Serial3.println("On attent");
  pere->setAttente(temps);
}


CommandeRestaurerTrajectoire::CommandeRestaurerTrajectoire(File<Trajectoire> *aRestaurer, Deplacement* ipere):_aRestaurer(aRestaurer), pere(ipere){
  
}
CommandeRestaurerTrajectoire::~CommandeRestaurerTrajectoire(){
  delete _aRestaurer; //a cet endroit, la file a déja été vidée dans la principale
}
void CommandeRestaurerTrajectoire::executer(){
  pere->fileTraj->concatener(_aRestaurer);
}

CommandeAvancer::CommandeAvancer(double iDistance, ParamTraj& iParams, Deplacement* iPere)
{
  distance = iDistance;
  pere = iPere;
  params = iParams;
}

CommandeAvancer::~CommandeAvancer()
{
  
}

void CommandeAvancer::executer()
{
  double xActuel = pere->asser.getX();
  double yActuel = pere->asser.getY();
  Angle thetaActuel = pere->asser.getTheta();

  double x = xActuel + cos(thetaActuel.versFloat()) * distance;
  double y = yActuel + sin(thetaActuel.versFloat()) * distance;

  if(distance < 0)
  {
    params.vitesseAvancer = - params.vitesseAvancer;
  }

  Trajectoire* traj = new Droite(xActuel, yActuel, x, y, params.vitesseAvancer, params.accAvancer);
  pere->ajouterFileTraj(traj);

  pere->setDernierX(x);
  pere->setDernierY(y);
  pere->setDernierTheta(thetaActuel);
}

CommandeRecalage::CommandeRecalage(double ixTheorique, double iyTheorique, Angle ithetaTheorique,double iTemps, Deplacement* iPere)
{
  xTheorique = ixTheorique;
  yTheorique = iyTheorique;
  thetaTheorique = ithetaTheorique;
  pere = iPere;
  tempsRecalage = iTemps;
}

CommandeRecalage::~CommandeRecalage()
{
  
}

void CommandeRecalage::executer()
{
  bool videGauche = true;
  bool videDroit = true;

  double xDest, yDest;

  pere->setTempsRecalage(tempsRecalage);
  pere->setVerification(false);

  bool coteVert = pere->getCoteVert();
  Serial.println(coteVert);
  if(!coteVert)
  {
    yTheorique = 3 - yTheorique;
    thetaTheorique = Angle(0) - thetaTheorique;
  }

  Serial.println("xTheorique " + String(xTheorique) + " yTheorique " + String(yTheorique)); 
  
  if(xTheorique < 0.11)
  {
    xDest = -0.02;
    yDest = yTheorique;
  }
  else if(xTheorique > 1.89)
  {
    xDest = 2.02;
    yDest = yTheorique;    
  }
  else if(yTheorique < 0.11)
  {
    xDest = xTheorique;
    yDest = -0.02;
  }
  else
  {
    xDest = xTheorique;
    yDest = 3.02;    
  }
  

  double xActuel = pere->asser.getX();
  double yActuel = pere->asser.getY();
  Trajectoire *traj = new Droite(xActuel, yActuel, xDest, yDest, -0.3, 0.05);
  Serial.println("x " + String(xDest) + " y " + String(yDest)); 
  pere->asser.nouvelleTrajectoire(traj);

  pere->setRecalage(micros());
  pere->setDernierX(xTheorique);
  pere->setDernierY(yTheorique);
  pere->setDernierTheta(thetaTheorique);
}

CommandeAjoutScore::CommandeAjoutScore(int iPoints, Deplacement *iPere)
{
  points = iPoints;
  pere = iPere;
}

CommandeAjoutScore::~CommandeAjoutScore()
{
  
}

void CommandeAjoutScore::executer()
{
  Serial.println("score");
  pere->ajouterScore(points);
}


