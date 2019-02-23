#include "Arduino.h"
#include "deplacement.h"
#include "servos.h"
#include "ecran.h"
#include "RobotDetector.h"

#define ROBOT_PRINCIPAL 0
#define ROBOT_SECONDAIRE 1
#define ROBOT_TEST 2

#define SERIAL_SERVOS Serial2

Deplacement::Deplacement(bool m_coteVert, double m_xInit, double m_yInit, Angle m_thetaInit, int const m_typeRobot)
  : asser(m_xInit, m_coteVert ? m_yInit : HAUTEUR_TERRAIN - m_yInit, m_coteVert ? m_thetaInit : Angle(0)-m_thetaInit,typeRobot),
  interface(SERIAL_SERVOS), sGrip(interface, 16), sPinceGauche(interface, 12), sPinceDroite(interface, 6), sFaux(interface, 1),
  pBarriere(interface, 8), pCroix(interface, 3), pDistributeur(interface, 11), pAbeille(interface,5), typeRobot(m_typeRobot)
{
  fileCommande = new File<Commande>();
  fileTraj = new File<Trajectoire>();

  tempsRecalage = 3;
  
  dernierX = m_xInit;
  dernierY = m_yInit;
  dernierTheta = m_thetaInit;

  finie = false;

  debutAttente = 0;
  tempsAttente = 0;

  coteVert = m_coteVert;

  score = 0;
}

Deplacement::~Deplacement()
{
  delete(fileCommande);
  delete(fileTraj);
}

void Deplacement::setRecalage(long iRecalage)
{
  debutRecalage = iRecalage;
}

void Deplacement::initServos()
{
  interface.begin(BAUDRATE);

  uint8_t statut;

  switch(typeRobot)
  {
    case ROBOT_PRINCIPAL :
      statut = pCroix.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Croix" + String(statut));
        Serial3.println("Erreur servo Croix" + String(statut));
      }
       pCroix.enableTorque();
       pCroix.jointMode(0, 1023);
       pCroix.speed(200);

       statut = pBarriere.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Barriere" + String(statut));
        Serial3.println("Erreur servo Barriere" + String(statut));
      }
       pBarriere.jointMode(0, 1023);
       pBarriere.enableTorque();
       pBarriere.speed(200);


       statut = pDistributeur.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Distributeur" + String(statut));
        Serial3.println("Erreur servo Distributeur" + String(statut));
      }
       pDistributeur.jointMode(0, 1023);
       pDistributeur.enableTorque();
       pDistributeur.speed(200);

       statut = pAbeille.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Abeille" + String(statut));
        Serial3.println("Erreur servo Distributeur" + String(statut));
      }
       pAbeille.jointMode(0, 1023);
       pAbeille.enableTorque();
       pAbeille.speed(200);

      break;

    case ROBOT_SECONDAIRE :
      statut = sGrip.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Grip" + String(statut));
      }

      sGrip.enableTorque();
      sGrip.speed(200);

      statut = sPinceGauche.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Pince gauche" + String(statut));
      }

      sPinceGauche.enableTorque();
      sPinceGauche.speed(200);

      statut = sPinceDroite.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Pince droite" + String(statut));
      }

      sPinceDroite.enableTorque();
      sPinceDroite.speed(200);

      statut = sFaux.init();
      if(statut!=DYN_STATUS_OK)
      {
        Ecran::getInstance().afficherTexte("Erreur servo Faux" + String(statut));
      }

      sFaux.enableTorque();
      sFaux.speed(200);
  }
}

void Deplacement::setTempsRecalage(double iTemps)
{
  tempsRecalage = iTemps;
}

void Deplacement::tournerAX12(DynamixelMotor& servo, int nbTours, int16_t vitesse ,Angle reference){
  Serial.println("nb tours");
  Serial.println(nbTours);
  fileCommande->ajouter(new CommandeTournerAX12(servo, nbTours, vitesse, reference, this));
  /*ajouterCommandeGenerale([this]{
    fileTraj->ajouter(new Rotation(asser.getX(), asser.getY(), asser.getTheta(), asser.getTheta(), parametresLents.vitesseRotation, parametresLents.accRotation));
  });*/
  attendre(7);
}

void Deplacement::arreterAX12(DynamixelMotor& servo){
  fileCommande->ajouter(new CommandeTournerAX12(servo, 0, 0, 0, this));
}

void Deplacement::arreterTousServos(){
  switch(typeRobot){
    case ROBOT_PRINCIPAL:
    pCroix.jointMode();
   //distributeur
    break;
    case ROBOT_SECONDAIRE:
    break;
    default:
    Serial.println("Arret servos : Robot inconnu");
  }
}

void Deplacement::setCoteVert(boolean m_coteVert, double m_xInit, double m_yInit, Angle m_thetaInit, int iEndStopGauche, int iEndStopDroit)
{
  asser.init(m_xInit, m_coteVert ? m_yInit : HAUTEUR_TERRAIN - m_yInit, m_coteVert ? m_thetaInit : Angle(0)-m_thetaInit);
  endStopGauche = iEndStopGauche;
  endStopDroit = iEndStopDroit;
  coteVert = m_coteVert;
  
  dernierX = m_xInit;
  dernierY = m_yInit;
  dernierTheta = m_thetaInit;
}

void Deplacement::avancer(double distance, ParamTraj& params)
{
  CommandeAvancer* com = new CommandeAvancer(distance, params, this);
  fileCommande->ajouter(com);
}

void Deplacement::deplacerAX12(DynamixelMotor &servo, Angle angle)
{
  CommandeAX12* com = new CommandeAX12(servo, angle, this);
  fileCommande->ajouter(com);
}

void Deplacement::commandeScore(int points)
{
  CommandeAjoutScore* com = new CommandeAjoutScore(points, this);
  fileCommande->ajouter(com);
}

void Deplacement::ajouterScore(int points)
{
  score += points;
  Ecran::getInstance().afficherTexte("Score actuel : " + String(score));
}

void Deplacement::sFauxHaute()
{
  deplacerAX12(sFaux, Angle(5.0/6*PI));
}

void Deplacement::sFauxBasse()
{
  deplacerAX12(sFaux,Angle(5.0/3*PI));
}

void Deplacement::sPincesHaute()
{
  deplacerAX12(sPinceDroite,Angle(0.83333*PI));
  deplacerAX12(sPinceGauche,Angle(0.83333*PI));
}

void Deplacement::sPincesPresqueHaute()
{
  deplacerAX12(sPinceDroite,Angle(5.85/6*PI));
  deplacerAX12(sPinceGauche,Angle(4.15/6*PI));
}

void Deplacement::sPincesBasse()
{
  deplacerAX12(sPinceDroite,Angle(4.305));
  deplacerAX12(sPinceGauche,Angle(0.883));
}

void Deplacement::sPincesPresqueBasse()
{
  deplacerAX12(sPinceDroite,Angle(4.02/3*PI));
  deplacerAX12(sPinceGauche,Angle(0.98/3*PI));
}

void Deplacement::sPincesInterrupteur()
{
  deplacerAX12(sPinceGauche,Angle(1.37));
  deplacerAX12(sPinceDroite,Angle(3.79));
}

void Deplacement::sGripFerme()
{
  deplacerAX12(sGrip,Angle(5.1/6*PI));
}

void Deplacement::sGripOuvert()
{
  deplacerAX12(sGrip,Angle(12.5/9*PI));
}

void Deplacement::sGripMid()
{
  deplacerAX12(sGrip,Angle(1.15*PI));
}

void Deplacement::pAbeilleRangee()
{
  deplacerAX12(pAbeille, Angle(4.934));
}

void Deplacement::pAbeilleGauche()
{
  deplacerAX12(pAbeille, Angle(2.));
}

void Deplacement::pAbeilleDeriere()
{
  deplacerAX12(pAbeille, Angle(0.845));
}

void Deplacement::pInitialiserCroix(){
  ajouterCommandeGenerale([&]{
    pCroix.jointMode(0, 1023);
    pCroix.goalPosition(512*1./3);
    delay(500);
    pCroix.speed(0);
    //pCroix.wheelMode();
  });
}

void Deplacement::pInitialiserDistributeur(){
  ajouterCommandeGenerale([&]{
    pDistributeur.jointMode(0, 1023);
    pDistributeur.goalPosition(512*1./3);
    delay(500);
    pDistributeur.speed(0);
    //pDistributeur.wheelMode();
  });
}
void Deplacement::pChargerBalles(){
  tournerAX12(pCroix, 2, 400);
}

void Deplacement::pSpinCroix(bool spin, int vitesse){
  if(spin){
    tournerAX12(pCroix, 2, vitesse);
  }else{
    arreterAX12(pCroix);
  }
}

void Deplacement::pOuvrirBarriere(){
  deplacerAX12(pBarriere,Angle(1./12*PI));
}

void Deplacement::pFermerBarriere(){
  deplacerAX12(pBarriere,Angle(13./12*PI));
}

void Deplacement::attendre(float temps)
{
  CommandeAttente* com = new CommandeAttente(temps,this);
  fileCommande->ajouter(com);
}

bool Deplacement::getCoteVert()
{
  return coteVert;
}

void Deplacement::goTo(double iX, double iY, ParamTraj& params, bool verification, Angle iTheta, bool angleImpose, bool marcheArriere)
{
  Serial.println("x " + String(iX) + " y " + String(iY));
  if(angleImpose)
  {
    CommandeMoteur* com = new CommandeMoteur(iX,iY,iTheta,true,marcheArriere,params,false, this);
    fileCommande->ajouter(com);
  }
  else
  {
    CommandeMoteur* com = new CommandeMoteur(iX,iY,iTheta,false,marcheArriere,params,verification, this);
    fileCommande->ajouter(com);
  }
}

void Deplacement::setVerification(bool iVerification)
{
  verification = iVerification;
}

void Deplacement::recalage(double xTheorique, double yTheorique, Angle thetaTheorique, double tempsRecalage)
{
  CommandeRecalage* com = new CommandeRecalage(xTheorique, yTheorique, thetaTheorique, tempsRecalage, this);
  fileCommande->ajouter(com);
  attendre(0.5);
}

void Deplacement::setDernierX(double x)
{
  dernierX = x;
}

void Deplacement::setDernierY(double y)
{
  dernierY = y;
}

void Deplacement::setDernierTheta(Angle theta)
{
  dernierTheta = theta;
}

void Deplacement::setAttente(float temps)
{
  debutAttente = micros();
  tempsAttente = temps;
}

void Deplacement::executionCommandes()
{
  while(!fileCommande->estVide())
  {
    Commande* commande = fileCommande->retirer();
    commande->executer();
    delete(commande);
  }
}


bool Deplacement::actualiser()
{
  if(debutRecalage != 0)
  {
    bool videGauche = digitalRead(endStopGauche);
    bool videDroit = digitalRead(endStopDroit);

    Serial3.println("Gauche " + String(videGauche) + " Droite " + String(videDroit));

    if((!videGauche && !videDroit) || micros() - debutRecalage > tempsRecalage * 1000000)
    {
      Serial3.println("Recalage terminé");

      asser.init(dernierX, dernierY, dernierTheta);

      Angle theta = asser.getTheta();

      asser.nouvelleTrajectoire(new Rotation(dernierX, dernierY, theta, theta, 0.1, 0.1));

      debutRecalage = 0;
    }
  }
  asser.actualise(micros());
  double x = asser.getX();
  double y = asser.getY();
  Angle theta = asser.getTheta();
  //Serial.println(asser.traj->erreurPos(x, y, theta, micros()));


  //detection robot adverse
  RobotLocation loc = RobotDetector::getInstance().detect(x, y, theta);
  /*Serial3.print("distance tot ");
  Serial3.println(loc.distance);*/
  if(loc.distance<=0.3 && verification){//    verification
    if(asser.getTrajectoireCourante()->TYPE==DROITE){
      const static ParamTraj parametresLents(0.5,1.5,0.15,1.5);
      asser.nouvelleTrajectoire(new Rotation(x, y, theta, theta, parametresLents.vitesseRotation, parametresLents.accRotation));
      Serial3.println("OBSTACLE !!");
      if(micros() - debutAttente > tempsAttente * 1000000){//si on n'est pas déja en train d'attendre
        if(!fileTraj->estVide()){
          File<Trajectoire> * fileTrajTmp = fileTraj;
          fileTraj = new File<Trajectoire>();
          //Ecran::getInstance().afficherTexte("Piche non vide");
          fileCommande->ajouterPriorite(new CommandeRestaurerTrajectoire(fileTrajTmp, this));
        }

        fileTraj->ajouterPriorite(new Droite(x, y, dernierX, dernierY, parametresLents.vitesseAvancer, parametresLents.accAvancer));
        debutAttente = micros();
        tempsAttente = 1;
      }
    }else if(micros() - debutAttente <= tempsAttente * 1000000){
      Serial3.println("Attente de fin d'OBSTACLE");
      debutAttente = micros();
      tempsAttente = 1;
    }
      /*pathfinder.setPositionAdversaire(x + cos(loc.angle.versFloat())*RobotDetector::DISTANCE_DETECTION,y + sin(loc.angle.versFloat())*RobotDetector::DISTANCE_DETECTION, 0.3);
      File<Trajectoire> * fileTrajTmp = fileTraj;
      fileTraj = new File<Trajectoire>();

      ParamTraj goto
      sEvitement;
      parametresEvitement.accAvancer = 0.1;
      parametresEvitement.vitesseAvancer = 0.2;
      parametresEvitement.accRotation = 0.5;
      parametresEvitement.vitesseRotation = 1;

      Droite& trajCourante = *(Droite*)asser.getTrajectoireCourante();

      pathfinder.findGoTo(x, y, trajCourante.getArriveeX(), trajCourante.getArriveeY(), parametresEvitement, *this);
      fileTraj->concatener(fileTrajTmp);
      delete fileTrajTmp;

      asser.nouvelleTrajectoire(fileTraj->retirer());
      */
  }

  if(micros() - debutAttente > tempsAttente * 1000000){
    bool trajFinie = asser.trajFinie(micros());
  	if(trajFinie && !fileTraj->estVide())
  	{
      finie = false;
      //verification = false;
      Serial3.println("Nouvelle trajectoire");
  		asser.nouvelleTrajectoire(fileTraj->retirer());
      Serial3.println(asser.getTrajectoireCourante()->TYPE);
  	}
    else if (trajFinie && !fileCommande->estVide())
    {
      Serial.println("Nouvelle commande");
      Commande* commande = fileCommande->retirer();
      commande->executer();
      delete(commande);
    }
  	else if(trajFinie && fileCommande->estVide() && fileTraj->estVide() && !finie)
  	{
      Serial3.println("Stratégie finie");
      Angle theta = asser.getTheta();
      ParamTraj params;
      asser.nouvelleTrajectoire(new Rotation(dernierX, dernierY, dernierTheta, theta, params.vitesseRotation, params.accRotation));
      finie = true;
  		return true;
  	}
  }

	return false;
}

void Deplacement::ajouterFileTraj(Trajectoire* traj)
{
  fileTraj->ajouter(traj);
}

void Deplacement::ajouterCommande(Commande *commande){
  fileCommande->ajouter(commande);
}

