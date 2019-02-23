#ifndef COMMANDE
#define COMMANDE

#include "angle.h"
#include "servos.h"
#include "ParamTraj.h"
#include "deplacement.h"
#include "commandegenerale.h"


class Deplacement;


class CommandeMoteur : public Commande
{
  public :
    CommandeMoteur(double iX, double iY, Angle iTheta, bool iAngleImpose, bool iMarcheArriere, ParamTraj& iParams, bool iVerification, Deplacement *iPere);
    ~CommandeMoteur();
    virtual void executer();

  private:
    Angle caculAngle(double xDepart, double xArrivee, double yDepart, double yArrivee);
    double x;
    double y;
    Angle theta;
    bool marcheArriere;
    bool angleImpose;
    Deplacement *pere;
    double valeurAbsolue(double initial);
    ParamTraj params;
    bool verification;
};

class CommandeAX12 : public Commande
{
  public:
    CommandeAX12(DynamixelMotor &servo, Angle &angle, Deplacement *iPere);
    ~CommandeAX12();
    virtual void executer();  
  private:
    Angle mAngle;
    DynamixelMotor &mServo;
    Deplacement *pere;
};

class CommandeTournerAX12 : public Commande
{
  public:
    //tourne un ax12 de nbTours, ou indéfiniment si nTours = 0
    CommandeTournerAX12(DynamixelMotor &servo, int nbTours, int16_t vitesse, Angle reference , Deplacement *iPere);
    ~CommandeTournerAX12();
    virtual void executer();  
  private:
    Angle _reference;
    int16_t _vitesse;
    int _nbTours;
    DynamixelMotor &mServo;
    Deplacement *pere;
};

class CommandeAttente : public Commande
{
  public:
    CommandeAttente(float iTemps, Deplacement* iPere);
    ~CommandeAttente();
    virtual void executer();

  private :
    float temps;
    Deplacement *pere;
};


class CommandeAvancer : public Commande
{
  public:
    CommandeAvancer(double iDistance, ParamTraj& iParams, Deplacement* iPere);
    ~CommandeAvancer();
    virtual void executer();

  private : 
    double distance;
    Deplacement *pere;
    ParamTraj params;
};

class CommandeRestaurerTrajectoire : public Commande
{
  public:
    CommandeRestaurerTrajectoire(File<Trajectoire> *aRestaurer, Deplacement *ipere);
    ~CommandeRestaurerTrajectoire();
    virtual void executer();

  private : 
    File<Trajectoire> *_aRestaurer;
    Deplacement *pere;
};

class CommandeRecalage : public Commande
{
  public:
    CommandeRecalage(double ixTheorique, double iyTheorique, Angle ithetaTheorique, double iTempsRecalage, Deplacement* iPere);
    ~CommandeRecalage();
    virtual void executer();

  private : 
    double xTheorique;
    double yTheorique;
    Angle thetaTheorique;
    Deplacement *pere;
    double tempsRecalage;
};

class CommandeAjoutScore : public Commande
{
  public:
    CommandeAjoutScore(int iPoints, Deplacement* iPere);
    ~CommandeAjoutScore();
    virtual void executer();

  private :
    int points;
    Deplacement *pere;
};

#endif
