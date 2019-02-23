#ifndef DEPLACEMENT
#define DEPLACEMENT

#include "angle.h"
#include "file.h"
#include "asservissement.h"
#include "trajectoire.h"
#include "ParamTraj.h"
#include "pathfinding.h"
#include "commandegenerale.h"
#include "commande.h"

#define HAUTEUR_TERRAIN 3

#define PLAGE_SERVOS 5.236

class Deplacement
{
    public:
    //PathFinder pathfinder;
    File<Trajectoire> *fileTraj;

    private:
    void deplacerAX12(DynamixelMotor &servo, Angle angle);


//fait tourner un AX12 de x tours, à vitesse constante
//si reference != 0, compte à partir de l'angle reference, sinon à partir de la position actuelle
//Le servo ne DOIT PAS être en butée sur ses limites, et être déja en wheelMode
//VITESSE != 0
    void tournerAX12(DynamixelMotor& servo, int nbTours, int16_t vitesse ,Angle reference = 0);

//arrete un AX12 qui est en mode roue
    void arreterAX12(DynamixelMotor& servo);

    File<Commande> *fileCommande;
    bool finie;
    double dernierX;
    double dernierY;
    Angle dernierTheta;
    long debutAttente;
    float tempsAttente;
    bool coteVert;
    int const typeRobot;

    double tempsRecalage;

    int endStopGauche;
    int endStopDroit;

    bool verification = false;
    long debutRecalage = 0;

    int score;

    const long unsigned int BAUDRATE = 57143;//idealement 57142.9
    HardwareDynamixelInterface interface;
    DynamixelMotor sGrip, sPinceGauche, sPinceDroite, sFaux;
    DynamixelMotor pBarriere, pCroix, pDistributeur, pAbeille;

    Droite* trajCourante = nullptr;

  public:
    Deplacement(bool m_coteVert, double m_xInit, double m_yInit, Angle m_thetaInit, int const m_typeRobot);
    ~Deplacement();

    void initServos();
    void setCoteVert(boolean coteVert, double xInit, double yInit, Angle thetaInit, int iEndStopGauche, int iEndStopDroit);

    void sFauxHaute();
    void sFauxBasse();
    void sPincesHaute();
    void sPincesPresqueHaute();
    void sPincesBasse();
    void sPincesPresqueBasse();
    void sPincesInterrupteur();
    void sGripFerme();
    void sGripOuvert();
    void sGripMid();

    void pAbeilleRangee();
    void pAbeilleDeriere();
    void pAbeilleGauche();
    void pInitialiserCroix();
    void pChargerBalles();
    void pOuvrirBarriere();
    void pFermerBarriere();
    void pInitialiserDistributeur();
    void pSpinCroix(bool spin, int vitesse=600);
    void pAbeilleRangeeLent(){
      pAbeille.speed(100);
      deplacerAX12(pAbeille, Angle(4.934));
    }

    bool getCoteVert();

    void ajouterScore(int points);
    void commandeScore(int points);

    void setRecalage(long iRecalage);

    void arreterTousServos();

    void goTo(double iX, double iY, ParamTraj& params, bool verification = true, Angle iTheta = Angle(0), bool angleImpose = false, bool marcheArriere=false);

     void goTo(double distance);

    void goTo(double iX, double iY, ParamTraj& params, bool verification, bool marcheArriere)
    {
      goTo(iX,iY,params, verification, Angle(0),false, marcheArriere);
    }


    void avancer(double distance,ParamTraj& params);
    void recalage(double xTheorique, double yTheorique, Angle thetaTheorique, double tempsRecalage = 3);

    void setTempsRecalage(double iTemps);

    void attendre(float temps);
	  bool actualiser();
    void setDernierX(double x);
    void setDernierY(double y);
    void setDernierTheta(Angle theta);
    void setAttente(float temps);
    void ajouterFileTraj(Trajectoire* traj);
    Asservissement asser;

    void setVerification(bool iVerification);

    void executionCommandes();

public:

    template <class Lambda>
    void ajouterCommandeGenerale(Lambda&& commande){
        fileCommande->ajouter(make_CommandeGenerale_p(forward(commande)));
    }

    void ajouterCommande(Commande *commande);
};

#endif
